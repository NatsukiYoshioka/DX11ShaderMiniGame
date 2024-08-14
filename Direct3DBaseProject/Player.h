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

	/// <summary>
	/// �I�u�W�F�N�g�̉�]�ʂ̎擾
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̉�]��</returns>
	float GetRotate()const { return m_rotate; }

private:
	DX::AnimationSDKMESH m_animation;			//�X�L�j���O�A�j���[�V�����N���X
	ModelBone::TransformArray m_drawBones;		//�{�[���ϊ��s��̔z��

	shared_ptr<OriginalEffect> m_effect;		//���f���`��p�G�t�F�N�g�N���X
	ComPtr<ID3D11InputLayout> m_inputLayout;	//���f���`��p���̓��C�A�E�g

	float m_rotate;		//���f����Y����]��
	Matrix m_world;		//���f���̃��[���h�s��
};

