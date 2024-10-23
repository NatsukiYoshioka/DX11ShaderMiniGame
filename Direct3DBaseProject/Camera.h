#pragma once
#include"GameObject.h"
using namespace DirectX::SimpleMath;

class GameObject;

/// <summary>
/// �J�����I�u�W�F�N�g
/// </summary>
class Camera:public GameObject
{
public:
	/// <summary>
	/// �J�����̏�����
	/// </summary>
	Camera();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~Camera();

	void InitializeTitle()override;

	/// <summary>
	/// �^�C�g���V�[���I�u�W�F�N�g�̍X�V
	/// </summary>
	void UpdateTitle()override;

	/// <summary>
	/// �^�C�g���V�[���I�u�W�F�N�g�̕`��(�`��Ȃ�)
	/// </summary>
	void DrawTitle()override;

	/// <summary>
	/// �I�u�W�F�N�g�̏�����
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// �I�u�W�F�N�g�̍X�V
	/// </summary>
	void Update()override;

	/// <summary>
	/// �I�u�W�F�N�g�̕`��(�J�����Ȃ̂ŕ`�揈���͖���)
	/// </summary>
	void Draw()override;

	/// <summary>
	/// ���U���g�V�[���I�u�W�F�N�g�̏�����
	/// </summary>
	void InitializeResult()override;

	/// <summary>
	/// ���U���g�V�[���I�u�W�F�N�g�̍X�V
	/// </summary>
	void UpdateResult()override;

	/// <summary>
	/// ���U���g�V�[���I�u�W�F�N�g�̕`��(�`��Ȃ�)
	/// </summary>
	void DrawResult()override;

	/// <summary>
	/// �e�̕`��(�`��Ȃ�)
	/// </summary>
	void DrawShadow()override{}

	/// <summary>
	/// �J�����r���[�s��̎擾
	/// </summary>
	Matrix GetView()const { return m_view; }

	/// <summary>
	/// �J�����v���W�F�N�V�����s��̎擾
	/// </summary>
	Matrix GetProjection()const { return m_projection; }

	/// <summary>
	/// �J�����̃s�b�`�擾
	/// </summary>
	float GetPitch()const  { return m_pitch; }

	/// <summary>
	/// �^�C�g���ł̃J�����̈ړ�����
	/// </summary>
	float GetTitlePosRatio()const { return m_titlePosRatio; }

	/// <summary>
	/// �^�C�g���ł̃J�����ړ����I���������擾
	/// </summary>
	bool GetIsFinishMoving()const { return m_isFinishMoving; }

private:
	Matrix m_view;			//�J�����̃r���[�s��
	Matrix m_projection;	//�J�����̃v���W�F�N�V�����s��
	float m_distance;		//�J�����ƃv���C���[�Ƃ̋���
	float m_pitch;			//�J�����̏c����]��
	float m_yaw;			//�J�����̉�����]��

	float m_titlePosRatio;	//�^�C�g���̍��W�ړ�����

	const float m_speed;		//�J�����̃f�t�H���g�X�s�[�h
	const float m_mouseSpeed;	//�J�����̃}�E�X�X�s�[�h

	const float m_minYaw;		//�J�����̍ŏ����[
	const float m_maxYaw;		//�J�����̍ő僈�[

	float m_moveWaitTime;				//�^�C�g���ł̃J�����ړ��܂ł̑ҋ@����
	const float m_maxMoveWaitTime;		//�^�C�g���ł̃J�����ړ��܂ł̍ő�ҋ@����
	bool m_isFinishMoving;				//�^�C�g���̃J�����ړ����I���������ǂ���
	const Vector3 m_titleInitializePos;	//�^�C�g���ł̏������W
	const Vector3 m_titleFinalPos;		//�^�C�g���ł̍ŏI���W
	const float m_titleSpeed;			//�^�C�g���ł̍��W�ړ��X�s�[�h

	const float m_initializePitch;		//�J�����̏����s�b�`
	const float m_initializeYaw;		//�J�����̏������[
	const float m_resultCameraHeight;	//���U���g�ł̃J�����̍���
};

