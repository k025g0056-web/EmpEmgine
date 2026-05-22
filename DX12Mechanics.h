#pragma once
#include <d3d12.h>
#include<cstdint>
enum class WhichResource {
	Vertex,
	Index,
	Constant
};

class DX12Mechanics {
public:
	static ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes, WhichResource resour);

};