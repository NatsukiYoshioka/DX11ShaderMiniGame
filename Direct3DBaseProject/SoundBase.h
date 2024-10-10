#pragma once
#include"GameObject.h"

using namespace DirectX;
using namespace std;

class GameObject;

/// <summary>
/// UI�̊��N���X
/// </summary>
class SoundBase :public GameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SoundBase() {}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SoundBase() {}

	//�������z�֐�
	virtual void InitializeTitle() = 0;
	virtual void UpdateTitle() = 0;
	void DrawTitle()override{}
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	void Draw()override{}
	virtual void InitializeResult() = 0;
	virtual void UpdateResult() = 0;
	void DrawResult()override{}

protected:
	unique_ptr<SoundEffect> m_sound;
	unique_ptr<SoundEffectInstance> m_soundInstance;
};