#pragma once
#include"GameObject.h"
#include"Animation.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace Microsoft::WRL;
using namespace std;

class GameObject;
class OriginalEffect;

/// <summary>
/// �G�l�~�[�I�u�W�F�N�g
/// </summary>
class Enemy :public GameObject
{
public:
	/// <summary>
	/// �A�j���[�V�����̏��
	/// </summary>
	enum class AnimationState
	{
		Idle,			//�ҋ@
		RightWalk,		//�E����
		LeftWalk		//������
	};

	/// <summary>
	/// �I�u�W�F�N�g�̏�����
	/// </summary>
	/// <param name="fileName">3D���f���t�@�C����</param>
	/// <param name="pos">�������W</param>
	/// <param name="rotate">������]��</param>
	Enemy(const wchar_t* fileName, Vector3 pos, float rotate);

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~Enemy();

	/// <summary>
	/// �I�u�W�F�N�g�̍X�V
	/// </summary>
	void Update()override;

	/// <summary>
	/// �I�u�W�F�N�g�̕`��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// �G�̖ڂ̈ʒu���擾
	/// </summary>
	/// <returns></returns>
	Vector3 GetEyePosition()const { return m_eyePos; }

	/// <summary>
	/// �G�̖ڂ̌������擾
	/// </summary>
	/// <returns></returns>
	Vector3 GetEyeDirection()const { return m_eyeDirection; }

	/// <summary>
	/// �G�̖ڂ̃r���[��ԍs��
	/// </summary>
	/// <returns></returns>
	Matrix GetEyeView()const { return m_eyeView; }

private:
	AnimationState m_nowAnimationState;			//�A�j���[�V�����̌��݂̏��
	vector<DX::AnimationSDKMESH> m_animations;	//�X�L�j���O�A�j���[�V�����N���X�R���e�i
	ModelBone::TransformArray m_drawBones;		//�{�[���ϊ��s��̔z��

	shared_ptr<OriginalEffect> m_effect;		//���f���`��p�G�t�F�N�g�N���X

	Vector3 m_eyePos;		//�G�̖ڂ̈ʒu
	Vector3 m_eyeDirection;	//�ڂ̌���
	Matrix m_eyeView;		//�G�̖ڂ̃r���[�s��

	float m_posAngle;	//���f���̍��W�����߂�p�x
	float m_rotate;		//���f����Y����]��
	Matrix m_world;		//���f���̃��[���h�s��

	bool m_isStartMoving;

	const float m_scale;	//���f���̃X�P�[��
	const float m_distance;	//���f���̋���
	const float m_speed;	//���f���̈ړ��X�s�[�h
};

