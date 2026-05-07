#include"ManagementCommand.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

void ManagementCommand::Initialize(ID3D12Device* device) {
	GenerateCommandQueue(device);
	GenerateCommandAllocator(device);
	GenerateCommandList(device);
	GenerateFence(device);
}

void ManagementCommand::GenerateCommandQueue(ID3D12Device* device) {
	//コマンドキューの生成
	commandQueueDesc_ = {};

	HRESULT hr_ = device->CreateCommandQueue(&commandQueueDesc_,
		IID_PPV_ARGS(&commandQueue_));
	//コマンドキューの生成が上手く行かなかったので実行出来ない
	assert(SUCCEEDED(hr_));
}

void ManagementCommand::GenerateCommandAllocator(ID3D12Device* device) {
	HRESULT hr_ = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator_));
	//コマンドアロケーターの生成がうまくいかなかったので生成できない
	assert(SUCCEEDED(hr_));
}

void ManagementCommand::GenerateCommandList(ID3D12Device* device) {
	HRESULT hr_ = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	//コマンドリストの生成が上手く行かなかったので起動できない
	assert(SUCCEEDED(hr_));
}

void ManagementCommand::LoadCommand(IDXGISwapChain4* swapChain,D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandles, ID3D12Resource** swapChainResources_) {
	//これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();
	//バリアを張る
	PutUpABarrier(swapChainResources_, backBufferIndex);
	//描画先のRTVを設定する
	commandList_->OMSetRenderTargets(1, &rtvHandles[backBufferIndex],
	false, nullptr);
	//指定した色で画面全体をクリアする
	commandList_->ClearRenderTargetView(rtvHandles[backBufferIndex], clearColor_,
		0, nullptr);
	PutUpReBarrier();
	//コマンドリストの内容を確定させる。全てのコマンドを積んでからCloseすること
	HRESULT hr = commandList_->Close();
	assert(SUCCEEDED(hr));
}

void ManagementCommand::KickCommand(IDXGISwapChain4* swapChain) {
	ID3D12CommandList* commandLists[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandLists);

	SendSignal();

	WaitingGPU();

	//GPUとOSに画面の交換を行うように通知する
	swapChain->Present(1, 0);
	//次フレーム用のコマンドリストを準備
	HRESULT hr = commandAllocator_->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr));
}

void ManagementCommand::PutUpABarrier(ID3D12Resource* swapChainResources_[], unsigned int index) {
	//今回のバリアはTransition
	barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier_.Transition.pResource = swapChainResources_[index];
	//遷移前（現在）のResourceState
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//遷移後のResource
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier_);

}

void ManagementCommand::PutUpReBarrier() {
	//画面に描く処理は全て終わり、画面に映すので、状態を遷移
	//今回はRenderTargetからPresentにする
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//TransitionBarrierを張る
	commandList_->ResourceBarrier(1, &barrier_);
}

void ManagementCommand::GenerateFence(ID3D12Device* device) {
	HRESULT hr = device->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr));

	fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent_!=nullptr);
}

void ManagementCommand::SendSignal() {
	//Fenceの値を更新
	fenceValue_++;
	//GPUがここまでたどりついたときにFenceの値を指定した値に代入するようにSignalを送る
	commandQueue_->Signal(fence_, fenceValue_);
}

void ManagementCommand::WaitingGPU() {
	//Fenceの値が指定したSignal値にたどりついてるか確認する
	//GetCompletedValueの初期値はFence作成時に渡した初期値
	if (fence_->GetCompletedValue()<fenceValue_) {
		//指定したSignalにたどりついていないので、たどりつくまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
		//イベントを待つ
		WaitForSingleObject(fenceEvent_, INFINITE);
	}
}