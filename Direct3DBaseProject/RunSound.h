#pragma once
#include"SoundBase.h"
#include"GameObject.h"

class SoundBase;

/// <summary>
/// ���葫���Ǘ��N���X
/// </summary>
class RunSound:public SoundBase
{
public:
	/// <summary>
	/// ���������̑����̏�����
	/// </summary>
	RunSound();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~RunSound();

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
	const float m_volume;	//�T�E���h�̃{�����[��
};

