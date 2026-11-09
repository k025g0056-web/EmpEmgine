#include"core/wrapper/EmpEngine.h"
#include"Scene/Manager/SceneManager.h"

//Windowsアプリでのエントリーポイント(main関数)

int WINAPI WinMain(HINSTANCE, HINSTANCE,LPSTR,int){
	//必ず最初に初期化する
	EmpEngine::Initialize(1280,720,SceneName::Play);

	//ゲームのメインループ
	while (EmpEngine::ProcessMessage()==0){
		//フレームの開始
		EmpEngine::Begin();

		//ゲームのプロセス
		EmpEngine::Process();

		//フレームの終了
		EmpEngine::End();

		//Escを押して終了
		if (EmpEngine::EndManagement()) {
			break;
		}

	}

	//インスタンスの解放
	EmpEngine::Finalize();

	return 0;
}