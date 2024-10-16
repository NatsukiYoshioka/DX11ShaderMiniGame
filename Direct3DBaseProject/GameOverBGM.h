#pragma once
#include"SoundBase.h"
#include"GameObject.h"

class SoundBase;

class GameOverBGM:public SoundBase
{
public:
	/// <summary>
	/// �Q�[���I�[�o�[BGM������
	/// </summary>
	GameOverBGM();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~GameOverBGM();

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
	float m_volume;
};

