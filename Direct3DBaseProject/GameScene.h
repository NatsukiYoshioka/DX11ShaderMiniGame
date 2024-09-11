#pragma once

class BaseScene;

class GameScene : public BaseScene
{
public:
	GameScene();
	~GameScene();

	void Update()override;
	void Draw()override;
	void DrawOffScreen()override;
};

