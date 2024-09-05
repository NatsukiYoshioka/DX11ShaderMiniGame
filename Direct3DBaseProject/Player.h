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
		CrouchedWalk	//���Ⴊ�ݕ���
	};

	/// <summary>
	/// �I�u�W�F�N�g�̏�����
	/// </summary>
	/// <param name="fileName">3D���f���t�@�C����</param>
	/// <param name="pos">�������W</param>
	/// <param name="rotate">�I�u�W�F�N�g��Y����]��(XM_PI / rotate)</param>
	Player(const wchar_t* fileName, Vector3 pos, float rotate);

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~Player();

	/// <summary>
	/// �I�u�W�F�N�g�̍X�V
	/// </summary>
	void Update()override;

	/// <summary>
	/// �I�u�W�F�N�g�̕`��
	/// </summary>
	void Draw()override;

	void DrawShadow();

	void DrawHitCheck();

	void HitCheck();

	/// <summary>
	/// �I�u�W�F�N�g�̉�]�ʂ̎擾
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̉�]��</returns>
	float GetRotate()const { return m_rotate; }

	bool GetBeFound()const { return m_beFound; }

	void HitCheckObject();

private:
	AnimationState m_nowAnimationState;			//�A�j���[�V�����̌��݂̏��
	vector<DX::AnimationSDKMESH> m_animations;	//�X�L�j���O�A�j���[�V�����N���X�R���e�i
	ModelBone::TransformArray m_drawBones;		//�{�[���ϊ��s��̔z��

	shared_ptr<OriginalEffect> m_effect;		//���f���`��p�G�t�F�N�g�N���X

	float m_rotate;		//���f����Y����]��
	Matrix m_world;		//���f���̃��[���h�s��

	struct HitInfo
	{
		int playerPixNum;
		int visiblePixNum;
	};
	ComPtr<ID3D11Buffer> m_bufferResult;
	ComPtr<ID3D11UnorderedAccessView> m_hitInfo;
	ComPtr<ID3D11ComputeShader> m_csForEnemyEye;

	struct Sphere
	{
		Vector3 center;
		float radius;
	};
	ComPtr<ID3D11Buffer> m_sphereResult;
	ComPtr<ID3D11UnorderedAccessView> m_sphereInfo;
	ComPtr<ID3D11ComputeShader> m_csForCollision;

	bool m_beFound;

	const float m_scale;
	const float m_speed;		//�v���C���[�̈ړ��X�s�[�h
	const float m_runSpeed;		//�v���C���[�̃_�b�V���X�s�[�h
	const float m_crouchSpeed;	//�v���C���[�̂��Ⴊ�݈ړ��X�s�[�h

	unique_ptr<SpriteBatch> batch;
	unique_ptr<SpriteFont> font;
};

