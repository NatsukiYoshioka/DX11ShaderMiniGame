#pragma once
#include"GameObject.h"

class GameObject;

/// <summary>
/// UI�̊��N���X
/// </summary>
class UIBase:public GameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	UIBase(){}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~UIBase(){}

	//�������z�֐�
	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:
	
};

