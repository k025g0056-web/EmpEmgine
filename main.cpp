#include"EmpEngine.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain( _In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR,_In_ int) {
	EmpEngine::Initialize();
	while (EmpEngine::ProcessMessage()==0){

	}

	EmpEngine::Finalize();
	return 0;
}