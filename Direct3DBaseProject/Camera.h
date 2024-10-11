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

	void Initialize()override;

	/// <summary>
	/// �I�u�W�F�N�g�̍X�V
	/// </summary>
	void Update()override;

	/// <summary>
	/// �I�u�W�F�N�g�̕`��(�J�����Ȃ̂ŕ`�揈���͖���)
	/// </summary>
	void Draw()override;

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
	/// �J�����r���[�s��̎擾
	/// </summary>
	/// <returns>�r���[�s��</returns>
	Matrix GetView()const { return m_view; }

	/// <summary>
	/// �J�����v���W�F�N�V�����s��̎擾
	/// </summary>
	/// <returns>�v���W�F�N�V�����s��</returns>
	Matrix GetProjection()const { return m_projection; }

	/// <summary>
	/// �J�����̃s�b�`�擾
	/// </summary>
	/// <returns>�J�����̃s�b�`</returns>
	float GetPitch()const  { return m_pitch; }

	float GetTitlePosRatio()const { return m_titlePosRatio; }

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

	const float m_minYaw;
	const float m_maxYaw;

	float m_moveWaitTime;
	const float m_maxMoveWaitTime;
	bool m_isFinishMoving;
	const Vector3 m_titleInitializePos;
	const Vector3 m_titleFinalPos;
	const float m_titleSpeed;

	const float m_initializePitch;
	const float m_initializeYaw;
	const float m_resultCameraHeight;
};

