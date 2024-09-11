#pragma once

class BaseScene;

class TitleScene:public BaseScene
{
public:
	TitleScene();
	~TitleScene();

	void Update()override;
	void Draw()override;
	void DrawOffScreen()override;

private:

};

