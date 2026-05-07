#include"ManagementDXGIFactory.h"

void ManagementDXGIFactory::Initialize() {

	HRESULT hr_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	//初期化の根本的な部分でエラーが出た場合はプログラムが間違っているか、
	// どうにも出来ない場合が多いのでassertにしておく
	assert(SUCCEEDED(hr_));
}

void ManagementDXGIFactory::DecideAdapter() {
	//良い順にアダプタを組む
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter_)) !=
		DXGI_ERROR_NOT_FOUND; ++i) {
		//アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		HRESULT hr_ = useAdapter_->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr_));//取得できないのは一大事
		//ソフトウェアアダプタでなければ採用‼
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//採用したアダプタの情報をログに出力。wstringの方なので注意
			ManagementLog::Log(std::format(L"USE Adapter:{}\n", adapterDesc.Description));
			break;
		}

		useAdapter_ = nullptr;//ソフトウェアの場合は見なかったことにする
	}

	//適切なアダプタがみつからなかったので起動できない
	assert(useAdapter_ != nullptr);
}

void ManagementDXGIFactory::Release() {
	useAdapter_->Release();
	dxgiFactory_->Release();
}