#include"EmpEngine.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	EmpEngine::Initialize();
	while (EmpEngine::ProcessMessage()==0){

	}

	EmpEngine::Finalize();
	return 0;
}