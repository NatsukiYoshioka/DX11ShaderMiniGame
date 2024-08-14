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
	/// �������z�֐�(�I�u�W�F�N�g�X�V�֐�)
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �������z�֐�(�I�u�W�F�N�g�`��֐�)
	/// </summary>
	virtual void Draw() = 0;

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

