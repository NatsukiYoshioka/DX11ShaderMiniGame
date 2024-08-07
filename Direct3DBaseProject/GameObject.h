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

protected:
	unique_ptr<Model> m_modelHandle;	//3D���f���n���h��
	Vector3 m_pos;						//�I�u�W�F�N�g�̍��W
};

