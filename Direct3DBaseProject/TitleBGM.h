#pragma once
#include"SoundBase.h"
#include"GameObject.h"

class SoundBase;

class TitleBGM:public SoundBase
{
public:
	/// <summary>
	/// �^�C�g��BGM�̏�����
	/// </summary>
	TitleBGM();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~TitleBGM();

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
	float m_volume;		//�T�E���h�̃{�����[��
};

