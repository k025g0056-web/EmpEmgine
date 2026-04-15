#include"EmpEngine.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//必ず最初に初期化する
	EmpEngine::Initialize();

	uint32_t* p = nullptr;
	*p = 100;
	//ゲームのメインループ
	while (EmpEngine::ProcessMessage()==0){

	}

	//インスタンスの解放
	EmpEngine::Finalize();
	return 0;
}