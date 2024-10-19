#pragma once
#include"GameObject.h"

using namespace std;
using namespace Microsoft::WRL;

class GameObject;
class OriginalEffect;

class Room:public GameObject
{
public:
	/// <summary>
	/// �I�u�W�F�N�g�̏�����
	/// </summary>
	/// <param name="fileName"></param>
	Room(const wchar_t* fileName);

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~Room();

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
	/// �e�̕`��
	/// </summary>
	void DrawShadow()override;

private:
	vector<ComPtr<ID3D11ShaderResourceView>> texture;		//�e�N�X�`���̃R���e�i
	vector<shared_ptr<OriginalEffect>> m_effect;			//�V�F�[�_�[�p�N���X

	Matrix m_world;		//���[���h�s��
	float m_rotate;		//�I�u�W�F�N�g�̉�]��
};

