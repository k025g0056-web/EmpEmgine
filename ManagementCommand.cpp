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

void ManagementCommand::LoadCommand(IDXGISwapChain4* swapChain,D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandles) {
	//これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();
	//描画先のRTVを設定する
	commandList_->OMSetRenderTargets(1, &rtvHandles[backBufferIndex], false, nullptr);
	//指定した色で画面全体をクリアする
	commandList_->ClearRenderTargetView(rtvHandles[backBufferIndex], clearColor_, 0, nullptr);
	//コマンドリストの内容を確定させる。全てのコマンドを積んでからCloseすること
	HRESULT hr = commandList_->Close();
	assert(SUCCEEDED(hr));
}

void ManagementCommand::KickCommand(IDXGISwapChain4* swapChain) {
	ID3D12CommandList* commandLists[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandLists);
	//GPUとOSに画面の交換を行うように通知する
	swapChain->Present(1, 0);
	//次フレーム用のコマンドリストを準備
	HRESULT hr = commandAllocator_->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr));
}