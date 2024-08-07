#pragma once
#include"GameObject.h"
using namespace DirectX::SimpleMath;

class GameObject;

/// <summary>
/// �J�����I�u�W�F�N�g
/// </summary>
class Camera:public GameObject
{
public:
	/// <summary>
	/// �J�����̏�����
	/// </summary>
	Camera();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~Camera();

	/// <summary>
	/// �I�u�W�F�N�g�̍X�V
	/// </summary>
	void Update()override;

	/// <summary>
	/// �I�u�W�F�N�g�̕`��(�J�����Ȃ̂ŕ`�揈���͖���)
	/// </summary>
	void Draw()override;

	/// <summary>
	/// �J�����r���[�s��̎擾
	/// </summary>
	/// <returns>�r���[�s��</returns>
	Matrix GetView()const { return m_view; }

	/// <summary>
	/// �J�����v���W�F�N�V�����s��̎擾
	/// </summary>
	/// <returns>�v���W�F�N�V�����s��</returns>
	Matrix GetProjection()const { return m_projection; }

private:
	Matrix m_view;			//�J�����̃r���[�s��
	Matrix m_projection;	//�J�����̃v���W�F�N�V�����s��
};

