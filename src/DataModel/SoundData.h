#pragma once
#include<windows.h>
#include<cstdint>

//チャンクヘッダ
struct ChunkHeader {
	char id[4];//チャンク前のID
	int32_t size;//チャンクのサイズ
};

//RIFFヘッダチャンク
struct RiffHeader {
	ChunkHeader chunk;//"RIFF"
	char type[4];//"WAVE"
};

//FMTチャンク
struct FormatChunk {
	ChunkHeader chunk;//"fmt"
	WAVEFORMATEX fmt;//波形フォーマット
};

//音声データ
struct SoundData {
	//波形データ
	WAVEFORMATEX wfex;
	//バッファの先頭アドレス
	BYTE* pBuffer;
	//バッファサイズ
	unsigned int bufferSize;
};
