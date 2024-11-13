#pragma once
#include"GameObject.h"

class GameObject;
class OriginalEffect;

/// <summary>
/// �l�`�̓��Ǘ��N���X
/// </summary>
class DollHead:public GameObject
{
public:
	/// <summary>
	/// �I�u�W�F�N�g�̏�����
	/// </summary>
	DollHead(const wchar_t* fileName, Vector3 pos);

	/// <summary>
	/// �I�u�W�F�N�g�̔j��
	/// </summary>
	~DollHead();

	/// <summary>
	/// �^�C�g���ł̏�����
	/// </summary>
	void InitializeTitle()override;

	/// <summary>
	/// �^�C�g���ł̍X�V
	/// </summary>
	void UpdateTitle()override;

	/// <summary>
	/// �^�C�g���ł̕`��
	/// </summary>
	void DrawTitle()override;

	/// <summary>
	/// ������
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// �I�u�W�F�N�g�̍X�V
	/// </summary>
	void Update()override;

	/// <summary>
	/// �I�u�W�F�N�g�̕`��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// ���U���g�ł̏�����
	/// </summary>
	void InitializeResult()override;

	/// <summary>
	/// ���U���g�ł̍X�V
	/// </summary>
	void UpdateResult()override;

	/// <summary>
	/// ���U���g�ł̕`��
	/// </summary>
	void DrawResult()override;

	/// <summary>
	/// �e�̕`��
	/// </summary>
	void DrawShadow()override;

	/// <summary>
	/// ���U���g�ł̍��W�ړ������擾
	/// </summary>
	float GetPosRatio()const { return m_posRatio; }

private:
	shared_ptr<OriginalEffect> m_effect;	//�V�F�[�_�[�`��p�N���X
	Matrix m_world;							//���[���h�s��
	float m_speed;							//�ړ��X�s�[�h
	float m_rotate;							//��]��
	float m_rotationSpeed;					//��]�X�s�[�h

	float m_posRatio;						//���W�ړ�����

	const float m_scale;					//���f���̃X�P�[��
	const Vector3 m_firstPos;				//�������W
	const Vector3 m_finalPos;				//�ŏI���W
	const float m_firstSpeed;				//�����X�s�[�h
	const float m_subSpeed;					//�X�s�[�h������
	const float m_firstRotationSpeed;		//������]�X�s�[�h
	const float m_subRotationSpeed;			//��]�X�s�[�h������
};