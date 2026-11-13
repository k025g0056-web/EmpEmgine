#define NOMINMAX
#include "ManagementFrameRate.h"
#include"externals/imgui/imgui.h"
#include <thread>
#include<algorithm>

void ManagementFrameRate::Initialize() {
	//現在の時間を真っ先に記録しておく
	reference_ = std::chrono::steady_clock::now();
}

void ManagementFrameRate::Update() {
	//指定したフレームレートピッタリの時間
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / maxFPS));
	//指定したフレームレートよりわずかに短い時間
	//const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / (minFPS+FPSOffset)));

	//現在の時間を取得する
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	//前回の記録からかかった時間を割り出す
	std::chrono::microseconds elaps =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	if (!isVsnc_) {
		//指定した時間に達していない場合
		if (elaps < kMinTime) {
			//時間がくるまで意図的に止める
			std::this_thread::sleep_until(reference_ + kMinTime);
		}

	}
	
	auto end = std::chrono::steady_clock::now();
	deltaTime = std::chrono::duration<float>(end - reference_).count();
	reference_ = end;
	deltaTime = std::min(deltaTime, 0.1f);
}

void ManagementFrameRate::GUI() {
	ImGui::Begin("FPS");
	if(isVsnc_) {
		if (ImGui::Button("Vsnc off")) {
			isVsnc_ = false;
		}
	}else {
		if (ImGui::Button("Vsnc on")) {
			isVsnc_ = true;
		}
	}

	ImGui::Text("preFPS: %.1f", ImGui::GetIO().Framerate);
	ImGui::DragFloat("Framerate", &maxFPS, 1.0f, 10.0f, 144.0f);
	ImGui::DragFloat("FrameOffset", &FPSOffset, 1.0f, 1.0f, 10.0f);
	ImGui::End();
}