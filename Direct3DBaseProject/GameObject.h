#pragma once
using namespace DirectX;
using namespace SimpleMath;
using namespace std;

/// <summary>
/// �Q�[���I�u�W�F�N�g���N���X
/// </summary>
class GameObject abstract
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	GameObject(){}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameObject(){}

	/// <summary>
	/// �������z�֐�(�^�C�g���V�[���I�u�W�F�N�g�������֐�)
	/// </summary>
	virtual void InitializeTitle() abstract;

	/// <summary>
	/// �������z�֐�(�^�C�g���V�[���I�u�W�F�N�g�X�V�֐�)
	/// </summary>
	virtual void UpdateTitle() abstract;

	/// <summary>
	/// �������z�֐�(�^�C�g���V�[���I�u�W�F�N�g�`��֐�)
	/// </summary>
	virtual void DrawTitle() abstract;

	/// <summary>
	/// �������z�֐�(�I�u�W�F�N�g�������֐�)
	/// </summary>
	virtual void Initialize() abstract;

	/// <summary>
	/// �������z�֐�(�I�u�W�F�N�g�X�V�֐�)
	/// </summary>
	virtual void Update() abstract;

	/// <summary>
	/// �������z�֐�(�I�u�W�F�N�g�`��֐�)
	/// </summary>
	virtual void Draw() abstract;

	/// <summary>
	/// �������z�֐�(���U���g�V�[���I�u�W�F�N�g�������֐�)
	/// </summary>
	virtual void InitializeResult() abstract;

	/// <summary>
	/// �������z�֐�(���U���g�V�[���I�u�W�F�N�g�X�V�֐�)
	/// </summary>
	virtual void UpdateResult() abstract;

	/// <summary>
	/// �������z�֐�(���U���g�V�[���I�u�W�F�N�g�`��֐�)
	/// </summary>
	virtual void DrawResult() abstract;

	/// <summary>
	/// �������z�֐�(�e�̕`��)
	/// </summary>
	virtual void DrawShadow() abstract;

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	Vector3 GetPos()const { return m_pos; }

	/// <summary>
	/// ���f���n���h���擾
	/// </summary>
	Model* GetModelHandle() { return m_modelHandle.get(); }

protected:
	unique_ptr<Model> m_modelHandle;	//3D���f���n���h��
	Vector3 m_pos;						//�I�u�W�F�N�g�̍��W
};

