#pragma once
using namespace DirectX;
using namespace SimpleMath;
using namespace std;

/// <summary>
/// �Q�[���I�u�W�F�N�g���N���X
/// </summary>
class GameObject
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
	virtual void InitializeTitle() = 0;

	/// <summary>
	/// �������z�֐�(�^�C�g���V�[���I�u�W�F�N�g�X�V�֐�)
	/// </summary>
	virtual void UpdateTitle() = 0;

	/// <summary>
	/// �������z�֐�(�^�C�g���V�[���I�u�W�F�N�g�`��֐�)
	/// </summary>
	virtual void DrawTitle() = 0;

	/// <summary>
	/// �������z�֐�(�I�u�W�F�N�g�������֐�)
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// �������z�֐�(�I�u�W�F�N�g�X�V�֐�)
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �������z�֐�(�I�u�W�F�N�g�`��֐�)
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// �������z�֐�(���U���g�V�[���I�u�W�F�N�g�������֐�)
	/// </summary>
	virtual void InitializeResult() = 0;

	/// <summary>
	/// �������z�֐�(���U���g�V�[���I�u�W�F�N�g�X�V�֐�)
	/// </summary>
	virtual void UpdateResult() = 0;

	/// <summary>
	/// �������z�֐�(���U���g�V�[���I�u�W�F�N�g�`��֐�)
	/// </summary>
	virtual void DrawResult() = 0;

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W(Vector3)</returns>
	Vector3 GetPos()const { return m_pos; }

	/// <summary>
	/// ���f���n���h���擾
	/// </summary>
	/// <returns>���f���̃|�C���^</returns>
	Model* GetModelHandle() { return m_modelHandle.get(); }

protected:
	unique_ptr<Model> m_modelHandle;	//3D���f���n���h��
	Vector3 m_pos;						//�I�u�W�F�N�g�̍��W
};

