#pragma once
#include<xaudio2.h>
#include<wrl.h>
#include<windows.h>
#include"SoundData.h"

class ManagementAudio{
	Microsoft::WRL::ComPtr<IXAudio2>xAudio2;
	IXAudio2MasteringVoice* mastervoice = nullptr;

public:
	void Initialize();

	SoundData SoundLoadWave(const char* filename);

	void SoundUnload(SoundData* soundData);

	void SoundPlayWave(const SoundData& soundData);

	void Release();
};

