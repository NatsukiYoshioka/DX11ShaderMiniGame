#pragma once

/// <summary>
/// �V�[���̊��N���X
/// </summary>
class BaseScene
{
public:
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DrawOffScreen() = 0;

	bool GetIsChangeScene()const { return m_isChangeScene; }
protected:
	bool m_isChangeScene;
};