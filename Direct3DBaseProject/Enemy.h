#pragma once
#include"GameObject.h"
#include"Animation.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace Microsoft::WRL;
using namespace std;

class GameObject;
class OriginalEffect;

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
	Enemy(const wchar_t* fileName, Vector3 pos, float rotate);
	~Enemy();

	void Update()override;
	void Draw()override;

private:
	AnimationState m_nowAnimationState;			//�A�j���[�V�����̌��݂̏��
	vector<DX::AnimationSDKMESH> m_animations;	//�X�L�j���O�A�j���[�V�����N���X�R���e�i
	ModelBone::TransformArray m_drawBones;		//�{�[���ϊ��s��̔z��

	shared_ptr<OriginalEffect> m_effect;		//���f���`��p�G�t�F�N�g�N���X

	float m_rotate;		//���f����Y����]��
	Matrix m_world;		//���f���̃��[���h�s��

	const float m_scale;	//���f���̃X�P�[��
};

