#pragma once
#include<xaudio2.h>
#include<wrl.h>
#include<windows.h>
#include"DataModel/SoundData.h"
#include<vector>

class ManagementAudio{
	Microsoft::WRL::ComPtr<IXAudio2>xAudio2;
	IXAudio2MasteringVoice* mastervoice = nullptr;
	std::vector<SoundData> soundDatas_;

public:
	void Initialize();
	int SoundLoadWave(const char* filename);

	void SoundUnload(int soundData);

	void SoundPlayWave(const int& soundData);

	void Release();
};

