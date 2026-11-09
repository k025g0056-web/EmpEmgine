#include "ManagementAudio.h"
#pragma comment(lib,"xaudio2.lib")
#include<fstream>
#include<cassert>

void ManagementAudio::Initialize() {
	HRESULT hr = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(hr));

	hr = xAudio2->CreateMasteringVoice(&mastervoice);
	assert(SUCCEEDED(hr));

}

SoundData ManagementAudio::SoundLoadWave(const char* filename) {
	
	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	std::string str = std::string("resources/sound/") + filename;

	//.wavファイルをバイナリモードで開く
	file.open(str.c_str(), std::ios_base::binary);

	//ファイルオープンを検出する
	assert(file.is_open());

	//RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	//ファイルがRIFFかチェックする
	if (strncmp(riff.chunk.id,"RIFF",4)!=0) {
		assert(0);
	}

	//タイプがWaveか調べる
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	FormatChunk format = {};
	//チャンクヘッダ―の確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	if (strncmp(data.id, "JUNK", 4) == 0) {

		//読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);

		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	//Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size] {};
	file.read(pBuffer, data.size);

	//Waveファイル
	file.close();

	SoundData soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;
}

void ManagementAudio::SoundUnload(SoundData* soundData) {
	delete[]soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void ManagementAudio::SoundPlayWave(const SoundData& soundData) {
	HRESULT result;

	//波形フォーマットを元にSourceVoiceを生成する
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2.Get()->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};

	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void ManagementAudio::Release() {
	xAudio2.Reset();
}