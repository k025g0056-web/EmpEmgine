#include"ManagementSwapChain.h"

void ManagementSwapChain::Initialize(int windowWidth, int windowHeight, ID3D12CommandQueue* commandQueue, HWND hwnd, IDXGIFactory7* dxgiFactory ) {
	swapChainDesc.Width = windowWidth;//画面の幅。ウィンドウのクライアント領域を同じ物にしておく
	swapChainDesc.Height = windowHeight;//画面の高さ。ウィンドウのクライアント領域を同じ物にしておく
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色の形式
	swapChainDesc.SampleDesc.Count = 1;//マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//描画のターゲットとして利用する
	swapChainDesc.BufferCount = 2;//ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//モニタに映したら中身を破棄
	//コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	HRESULT hr = dxgiFactory->CreateSwapChainForHwnd(commandQueue, hwnd, &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swapChain_));
	assert(SUCCEEDED(hr));
}