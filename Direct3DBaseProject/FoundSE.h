#pragma once
#include"SoundBase.h"
#include"GameObject.h"

class SoundBase;

/// <summary>
/// �����������̃T�E���h�G�t�F�N�g�Ǘ��N���X
/// </summary>
class FoundSE:public SoundBase
{
public:
	/// <summary>
	/// �T�E���h�̏�����
	/// </summary>
	FoundSE();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~FoundSE();

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
	float m_pitch;			//�T�E���h�̃s�b�`
	const float m_volume;	//�T�E���h�̃{�����[��
};

