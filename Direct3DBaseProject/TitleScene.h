#pragma once
#include"BaseScene.h"

class BaseScene;

class TitleScene:public BaseScene
{
public:
	TitleScene();
	~TitleScene();

	void Update()override;
	void Draw()override;
	void DrawOffScreen()override;
	

	bool GetIsStartGame()const { return m_isStartGame; }
private:
	bool m_isStartGame;
};

