#define NOMINMAX
#include "Particle3D.h"
#include "Internal/DX12Mechanics/DX12Mechanics.h"
#include "DataModel/Matrix4x4.h"
#include <cmath>
#include <algorithm>
#include"core/internal/EmpSystems.h"

void Particle3D::Initialize(ID3D12Device* device, const int& modelData) {
    Shape::Initialize(device, true);
    modelData_ = ResolveModel(modelData);
    workVertices_ = ResolveModel(modelData);
    size_t meshCount = modelData_.meshes.size();
    kInstanceSrvIndex_ = EmpSystems::GetInstance()->GetHeapId();
    // ← デバッグ用に追加
    char buf[256];
    sprintf_s(buf, "MeshCount=%zu\n", meshCount);
    OutputDebugStringA(buf);
    for (size_t i = 0; i < meshCount; ++i) {
        sprintf_s(buf, "  Mesh[%zu] vertexCount=%zu materialName=%s\n",
            i, modelData_.meshes[i].vertices.size(),
            modelData_.meshes[i].materialName.c_str());
        OutputDebugStringA(buf);
    }
    vertexResources_.resize(meshCount);
    vertexBufferViews_.resize(meshCount);
    vertexDataPtrs_.resize(meshCount);

    for (size_t i = 0; i < meshCount; ++i) {
        const auto& mesh = modelData_.meshes[i];
        size_t vertexCount = mesh.vertices.size();

        // CreateBufferResourceは参照カウント1の生ポインタを返すので
        // Attach()で所有権だけ引き継ぐ(operator=だとAddRefされてリークする)
        vertexResources_[i].Attach(DX12Mechanics::CreateBufferResource(
            device, sizeof(VertexData) * vertexCount));

        // ComPtrは生ポインタへの暗黙変換を持たないのでGet()で取り出す
        vertexBufferViews_[i] = DX12Mechanics::GenerateVertexBufferView<VertexData>(
            vertexResources_[i].Get(), vertexCount);

        vertexDataPtrs_[i] = nullptr;
        vertexResources_[i]->Map(
            0, nullptr, reinterpret_cast<void**>(&vertexDataPtrs_[i]));
        std::memcpy(vertexDataPtrs_[i], mesh.vertices.data(),
            sizeof(VertexData) * vertexCount);
    }

    instancingResource_.Attach(DX12Mechanics::CreateBufferResource(device,
        sizeof(ParticleForGPU)*kMaxInstance));
    instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
    for (uint32_t i = 0; i < kMaxInstance;++i) {
        instancingData_[i].WVP = MakeIdentity4x4();
        instancingData_[i].world = MakeIdentity4x4();
    }

    //SRV
    const UINT size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    D3D12_CPU_DESCRIPTOR_HANDLE cpu = srvHeap_->GetCPUDescriptorHandleForHeapStart();
    cpu.ptr += size * kInstanceSrvIndex_;
    instancingSrvGpu_ = srvHeap_->GetGPUDescriptorHandleForHeapStart();
    instancingSrvGpu_.ptr += size * kInstanceSrvIndex_;

    D3D12_SHADER_RESOURCE_VIEW_DESC srv{};
    srv.Format = DXGI_FORMAT_UNKNOWN;
    srv.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srv.Buffer.FirstElement = 0;
    srv.Buffer.NumElements = kMaxInstance;
    srv.Buffer.StructureByteStride = sizeof(ParticleForGPU);
    srv.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    device->CreateShaderResourceView(instancingResource_.Get(), &srv, cpu);
}

void Particle3D::Draw(
    ID3D12GraphicsCommandList* commandList,
    const Camera3d& camera,
    const std::vector<Transform3d>& transform,
    const std::vector<Vector4>& color, int SpawnCount){

    commandList->SetGraphicsRootSignature(pipeline_->GetRootSignature());
    commandList->SetPipelineState(pipeline_->GetPSO(kBlendModeAdd));

    SpawnCount = std::min<int>(SpawnCount, kMaxInstance);

    for (int i = 0; i < SpawnCount;++i) {
        Transform3d t = transform[i];

        instancingData_[i].WVP = modelData_.meshes[0].rootNode.localMatrix * camera.GetWvp(t);
        instancingData_[i].world = modelData_.meshes[0].rootNode.localMatrix * Affine(t);
        instancingData_[i].color = color[i];
    }

    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
    commandList->SetGraphicsRootDescriptorTable(1, instancingSrvGpu_);
   
    for (size_t i = 0; i < modelData_.meshes.size(); ++i) {
        int mat = modelData_.meshes[i].materialIndex;
        int tex = modelData_.materials[mat].texHandle;
        commandList->SetGraphicsRootDescriptorTable(2, ResolveTexture(tex));
        DrawCallVertexOnly(commandList, vertexBufferViews_[i],
            static_cast<int>(modelData_.meshes[i].vertices.size()), SpawnCount);
    }
}