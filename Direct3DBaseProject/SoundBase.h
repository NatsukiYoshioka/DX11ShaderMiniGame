#pragma once
#include"GameObject.h"

using namespace DirectX;
using namespace std;

class GameObject;

/// <summary>
/// UI�̊��N���X
/// </summary>
class SoundBase abstract:public GameObject
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
	virtual void InitializeTitle() abstract;
	virtual void UpdateTitle() abstract;
	void DrawTitle()override{}
	virtual void Initialize() abstract;
	virtual void Update() abstract;
	void Draw()override{}
	virtual void InitializeResult() abstract;
	virtual void UpdateResult() abstract;
	void DrawResult()override{}
	void DrawShadow()override{}

	/// <summary>
	/// �T�E���h�̏�Ԏ擾
	/// </summary>
	SoundState GetSoundState()const { return m_soundInstance->GetState(); }

protected:
	unique_ptr<SoundEffect> m_sound;					//�T�E���h�G�t�F�N�g�N���X
	unique_ptr<SoundEffectInstance> m_soundInstance;	//�T�E���h�G�t�F�N�g�̃C���X�^���X�N���X
};