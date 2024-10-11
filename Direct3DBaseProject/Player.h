#pragma once
#include"GameObject.h"
#include"Animation.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace Microsoft::WRL;
using namespace std;

class GameObject;
class OriginalEffect;

class Player :public GameObject
{
public:
	/// <summary>
	/// �A�j���[�V�����̏��
	/// </summary>
	enum class AnimationState
	{
		Idle,			//�ҋ@
		Walk,			//����
		Run,			//����
		Crouch,			//���Ⴊ��
		CrouchedWalk,	//���Ⴊ�ݕ���
		Dance,			//�Q�[���N���A
		Die				//�Q�[���I�[�o�[
	};

	/// <summary>
	/// �I�u�W�F�N�g�̏�����
	/// </summary>
	/// <param name="fileName">3D���f���t�@�C����</param>
	/// <param name="pos">�������W</param>
	/// <param name="rotate">�I�u�W�F�N�g��Y����]��(XM_PI / rotate)</param>
	Player(const wchar_t* fileName);

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~Player();

	/// <summary>
	/// �^�C�g���V�[���I�u�W�F�N�g�̏�����
	/// </summary>
	void InitializeTitle()override;

	/// <summary>
	/// �^�C�g���V�[���I�u�W�F�N�g�̍X�V
	/// </summary>
	void UpdateTitle()override;

	/// <summary>
	/// �^�C�g���V�[���I�u�W�F�N�g�̕`��
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
	/// �I�u�W�F�N�g�̕`��
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
	/// ���U���g�V�[���I�u�W�F�N�g�̕`��
	/// </summary>
	void DrawResult()override;

	/// <summary>
	/// �e�p�`��
	/// </summary>
	void DrawShadow();

	/// <summary>
	/// �����蔻��p�`��
	/// </summary>
	void DrawHitCheck();

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	void HitCheck();

	/// <summary>
	/// �I�u�W�F�N�g�̉�]�ʂ̎擾
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̉�]��</returns>
	float GetRotate()const { return m_rotate; }

	/// <summary>
	/// �G�Ɍ����������ǂ����擾
	/// </summary>
	/// <returns></returns>
	bool GetBeFound()const { return m_beFound; }

	bool GetIsClear()const { return m_isClear; }

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	void HitCheckObject();

	AnimationState GetNowAnimationState()const { return m_nowAnimationState; }

private:
	AnimationState m_nowAnimationState;			//�A�j���[�V�����̌��݂̏��
	vector<DX::AnimationSDKMESH> m_animations;	//�X�L�j���O�A�j���[�V�����N���X�R���e�i
	ModelBone::TransformArray m_drawBones;		//�{�[���ϊ��s��̔z��

	shared_ptr<OriginalEffect> m_effect;		//���f���`��p�G�t�F�N�g�N���X

	float m_rotate;		//���f����Y����]��
	Matrix m_world;		//���f���̃��[���h�s��

	/// <summary>
	/// �����蔻����
	/// </summary>
	struct HitInfo
	{
		int playerPixNum;	//�����������̃v���C���[�̃s�N�Z����
		int visiblePixNum;	//�f�t�H���g�s�N�Z����
	};
	ComPtr<ID3D11Buffer> m_bufferResult;			//�����蔻����o�͗p�o�b�t�@
	ComPtr<ID3D11UnorderedAccessView> m_hitInfo;	//�����蔻��pUAV
	ComPtr<ID3D11ComputeShader> m_csForEnemyEye;	//�����蔻��p�R���s���[�g�V�F�[�_�[

	/// <summary>
	/// �����蔻��p�X�t�B�A
	/// </summary>
	struct Sphere
	{
		Vector3 center;
		float radius;
	};
	ComPtr<ID3D11Buffer> m_sphereResult;			//�X�t�B�A�̓��o�̓o�b�t�@
	ComPtr<ID3D11UnorderedAccessView> m_sphereInfo;	//�X�t�B�A�pUAV
	ComPtr<ID3D11ComputeShader> m_csForCollision;	//�����蔻��p�R���s���[�g�V�F�[�_�[

	float m_sphereHeight;
	const float m_sphereRadius;
	const float m_sphereDefaultHeight;
	const float m_sphereCrouchHeight;

	bool m_beFound;		//�����������ǂ���
	bool m_isClear;		//�N���A�������ǂ���

	const Vector3 m_initializeTitlePos;
	const float m_titleRotateX;
	const float m_titleRotateY;
	const float m_titleScale;

	const Vector3 m_initializePos;		//�Q�[���V�[���̃I�u�W�F�N�g�������W
	const float m_initializeRotate;		//�Q�[���V�[���̃I�u�W�F�N�g������]��
	const float m_scale;				//�v���C���[�̃X�P�[��
	const float m_speed;				//�v���C���[�̈ړ��X�s�[�h
	const float m_runSpeed;				//�v���C���[�̃_�b�V���X�s�[�h
	const float m_crouchSpeed;			//�v���C���[�̂��Ⴊ�݈ړ��X�s�[�h

	const Vector3 m_clearPos;
	const Vector3 m_clearFinalPos;
};

