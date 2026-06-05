#pragma once

class Scene {
protected:
	int windowWidth_ = 0;
	int windowHeight_ = 0;

public:
	virtual ~Scene() = default;

	virtual void Initialize();

	virtual void Update();

	virtual void Draw();

	void SetWindowWidth(int windowWidth) { windowWidth_ = windowWidth; }
	void SetWindowHeight(int windowHeight) { windowHeight_ = windowHeight; }


};
