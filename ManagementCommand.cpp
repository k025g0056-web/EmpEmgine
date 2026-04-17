#include"ManagementCommand.h"

void ManagementCommand::Initialize(ID3D12Device* device) {
	//コマンドキューの生成
	commandQueueDesc_ = {};
	HRESULT hr_ = device->CreateCommandQueue(&commandQueueDesc_, IID_PPV_ARGS(&commandQueue_));
	//コマンドキューの生成が上手く行かなかったので実行出来ない
	assert(SUCCEEDED(hr_));

	hr_ = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	//コマンドアロケーターの生成がうまくいかなかったので生成できない
	assert(SUCCEEDED(hr_));

	hr_ = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	//コマンドリストの生成が上手く行かなかったので起動できない
	assert(SUCCEEDED(hr_));
}