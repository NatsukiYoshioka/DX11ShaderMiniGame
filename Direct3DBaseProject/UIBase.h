#pragma once
#include"GameObject.h"

class GameObject;

/// <summary>
/// UI�̊��N���X
/// </summary>
class UIBase abstract :public GameObject
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
	virtual void InitializeTitle() abstract;
	virtual void UpdateTitle() abstract;
	virtual void DrawTitle() abstract;
	virtual void Initialize() abstract;
	virtual void Update() abstract;
	virtual void Draw() abstract;
	virtual void InitializeResult() abstract;
	virtual void UpdateResult() abstract;
	virtual void DrawResult() abstract;
	void DrawShadow()override{}

protected:

};

