#include"ManagementSwapChain.h"

void ManagementSwapChain::Initialize(int windowWidth, int windowHeight, ID3D12CommandQueue* commandQueue, HWND hwnd, IDXGIFactory7* dxgiFactory ) {
	swapChainDesc_ = {};
	swapChainDesc_.Width = static_cast<UINT>(windowWidth);//画面の幅。ウィンドウのクライアント領域を同じ物にしておく
	swapChainDesc_.Height = static_cast<UINT>(windowHeight);//画面の高さ。ウィンドウのクライアント領域を同じ物にしておく
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色の形式
	swapChainDesc_.SampleDesc.Count = 1;//マルチサンプルしない
	swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//描画のターゲットとして利用する
	swapChainDesc_.BufferCount = 2;//ダブルバッファ
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//モニタに映したら中身を破棄
	//コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	HRESULT hr = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue, hwnd, &swapChainDesc_, nullptr,
		nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

void ManagementSwapChain::Release() {
	swapChain_.Reset();
	swapChain_ = nullptr;
}