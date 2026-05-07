#include"ManagementDevice.h"

void ManagementDevice::CreateDevice(IDXGIAdapter4* useAdaptor) {
	//機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};

	const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
	//高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		//採用したアダプターでデバイスを生成
		HRESULT hr_ = D3D12CreateDevice(useAdaptor, featureLevels[i], IID_PPV_ARGS(&device_));
		//指定した機能レベルでデバイスが生成出来たかを確認
		if (SUCCEEDED(hr_)) {
			//生成出来たのでログ出力を行ってループを抜ける
			ManagementLog::Log(std::format("FeatureLevel:{}\n", featureLevelStrings[i]));
			break;
		}
	}

	//デバイスの生成が上手く行かなかったので起動できない
	assert(device_ != nullptr);
	ManagementLog::Log("Complete create D3D12Device!!!\n");
}

void ManagementDevice::Release() {
	device_->Release();
	device_ = nullptr;
}