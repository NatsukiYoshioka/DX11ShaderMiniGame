#pragma once
#include"SoundBase.h"
#include"GameObject.h"

class SoundBase;

class GameBGM:public SoundBase
{
public:
	/// <summary>
	/// �Q�[���V�[��BGM�̏�����
	/// </summary>
	GameBGM();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~GameBGM();

	/// <summary>
	/// �^�C�g���T�E���h������
	/// </summary>
	void InitializeTitle()override;

	/// <summary>
	/// �^�C�g���T�E���h�X�V
	/// </summary>
	void UpdateTitle()override;

	/// <summary>
	/// �Q�[���V�[���T�E���h������
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// �Q�[���V�[���T�E���h�X�V
	/// </summary>
	void Update()override;

	/// <summary>
	/// ���U���g�T�E���h������
	/// </summary>
	void InitializeResult()override;

	/// <summary>
	/// ���U���g�T�E���h�X�V
	/// </summary>
	void UpdateResult()override;

private:
	float m_volume;					//�T�E���h�̃{�����[��
	const float m_initializeVolume;	//�T�E���h�̏����{�����[��
};

