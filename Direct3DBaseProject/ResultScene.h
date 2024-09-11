#pragma once

class BaseScene;

class ResultScene : public BaseScene
{
public:
	ResultScene();
	~ResultScene();

	void Update()override;
	void Draw()override;
	void DrawOffScreen()override;
};

