#include"EmpEngine.h"

//Windowsアプリでのエントリーポイント(main関数)

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//必ず最初に初期化する
	EmpEngine::Initialize(1280,720);
	
	//ゲームのメインループ
	while (EmpEngine::ProcessMessage()==0){
		EmpEngine::Begin();
		EmpEngine::Update();
		EmpEngine::DrawSpriteHomework();

		EmpEngine::DrawSphereHomework();
		EmpEngine::End();
	}

	//インスタンスの解放
	EmpEngine::Finalize();

	return 0;
}