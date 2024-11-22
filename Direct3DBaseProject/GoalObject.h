#pragma once
#include"GameObject.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace Microsoft::WRL;
using namespace std;

class GameObject;
class OriginalEffect;

class GoalObject:public GameObject
{
public:
	/// <summary>
	/// �I�u�W�F�N�g�̏�����
	/// </summary>
	GoalObject(const wchar_t* fileName, Vector3 pos, float rotate);

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~GoalObject();

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
	void DrawShadow()override;

private:
	shared_ptr<OriginalEffect> m_effect;		//�`��p�V�F�[�_�[
	Matrix m_world;								//���[���h���W�s��

	const float m_fixPosZ;		//Z��������
	const Vector3 m_scale;		//�I�u�W�F�N�g�̃X�P�[��
};

