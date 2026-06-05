#include"Particle.h"
#include"EmpEngine.h"
#include<cmath>
#include"externals/imgui/imgui.h"
#include<numbers>

void Particle::Initialize() {
	// YとZを入れ替えるの！
	verticeTri_[0] = { -0.2f, -0.2f,0.0f};
	verticeTri_[1] = { 0.0f, 0.2f ,0.0f};
	verticeTri_[2] = { 0.2f, -0.2f,0.0f };


	color = { 1.0f,0.8f,0.85f,1.0f };
	isPlayBack = false;
	time = 0.0f;
	length = 3.0f;
	for (int i = 0; i < maxAmount;i++) {
		float angle = static_cast<float>(i) / maxAmount * 2.0f * std::numbers::pi_v<float>;
		float r = length * (0.5f + 0.5f * static_cast<float>(std::sin(i * 1.7f)));

		startPositions[i].x = r * static_cast<float>(std::cos(angle));
		startPositions[i].z = r * static_cast<float>(std::sin(angle));
		startPositions[i].y = startY + static_cast<float>(i ) * 0.5f; 

		fallSpeed[i] = 0.01f + 0.01f * static_cast<float>(i % 5);
		swaySpeed[i] = 0.02f + 0.005f * static_cast<float>(i % 7);
		swayAmount[i] = 0.3f + 0.2f * static_cast<float>(i % 3);
		swayOffset[i] = static_cast<float>(i) * 0.5f;

		transform[i] = {
			{ 0.3f, 0.3f, 0.3f }, 
			{ 0.0f, 0.0f, 0.0f },
			startPositions[i]
		};
	}
}

void Particle::GUI() {

	ImGui::Checkbox("IsPlayBack", &isPlayBack);
	ImGui::DragFloat("radius", &length, 1.0f, 1.0f, 5.0f, "%.3f", 0);
	ImGui::ColorEdit4("color", &color.x, 0);
	if (ImGui::Button("Reset")) {
		isPlayBack = false;
		time = 0.0f;
		Initialize();
	}

	if (isPlayBack) {
		time += 0.15f;

		for (int i = 0; i < maxAmount; i++) {
			transform[i].translate.x = startPositions[i].x+ swayAmount[i] * static_cast<float>(std::sin(time * swaySpeed[i] * 10.0f + swayOffset[i]));

			transform[i].translate.y = startPositions[i].y - fallSpeed[i] * time * 20.0f;

			transform[i].rotate.y = time * swaySpeed[i] * 5.0f + swayOffset[i];
			transform[i].rotate.x = 0.3f * static_cast<float>(std::sin(time * swaySpeed[i] * 8.0f));

		
			if (transform[i].translate.y < -3.0f) {
				startPositions[i].y = startY + static_cast<float>(i) * 0.3f;
				
			}
		}
	}
}

void Particle::Draw() {
	for (int i = 0; i < maxAmount;i++) {
		EmpEngine::DrawTriangleColor(transform[i], verticeTri_[0], verticeTri_[1], verticeTri_[2], color);
	}
}