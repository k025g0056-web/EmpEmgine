#include "ManagementFrameRate.h"
#include"externals/imgui/imgui.h"
#include <thread>

void ManagementFrameRate::Initialize() {
	//現在の時間を真っ先に記録しておく
	reference_ = std::chrono::steady_clock::now();
}

void ManagementFrameRate::Update() {
	GUI();
	//指定したフレームレートピッタリの時間
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / minFPS));
	//指定したフレームレートよりわずかに短い時間
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / (minFPS+FPSOffset)));

	//現在の時間を取得する
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	//前回の記録からかかった時間を割り出す
	std::chrono::microseconds elaps =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	//指定した時間に達していない場合
	if (elaps<kMinTime) {
		//時間がくるまで意図的に止める
		while (std::chrono::steady_clock::now()-reference_<kMinTime) {
			//1マイクロ秒刻みで機能を止める
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}

	}

	reference_ = std::chrono::steady_clock::now();
}

void ManagementFrameRate::GUI() {
	ImGui::Begin("FPS");
	ImGui::Text("preFPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::DragFloat("Framerate", &minFPS, 1.0f, 1.0f, 144.0f);
	ImGui::DragFloat("FrameOffset", &FPSOffset, 1.0f, 1.0f, 10.0f);
	ImGui::End();
}