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

	SoundState GetSoundState()const { return m_soundInstance->GetState(); }

protected:
	unique_ptr<SoundEffect> m_sound;					//�T�E���h�G�t�F�N�g�N���X
	unique_ptr<SoundEffectInstance> m_soundInstance;	//�T�E���h�G�t�F�N�g�̃C���X�^���X�N���X
};