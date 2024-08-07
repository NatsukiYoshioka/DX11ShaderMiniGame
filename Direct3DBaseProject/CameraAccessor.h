#pragma once

class Camera;

/// <summary>
/// �J�����̃A�N�Z�T
/// </summary>
class CameraAccessor
{
private:
	/// <summary>
	/// �J�����̐���
	/// </summary>
	CameraAccessor();

	/// <summary>
	/// �J�����̔j��
	/// </summary>
	~CameraAccessor();
public:
	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// �C���X�^���X�j��
	/// </summary>
	static void DestroyInstance();

	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns>�A�N�Z�T�̃C���X�^���X</returns>
	static CameraAccessor* GetInstance() { return m_instance; }
	
	/// <summary>
	/// �J�����̎擾
	/// </summary>
	/// <returns>�J�����̃C���X�^���X</returns>
	Camera* GetCamera() { return m_camera; }
private:
	static CameraAccessor* m_instance;		//�A�N�Z�T�̃C���X�^���X
	Camera* m_camera;						//�J�����̃C���X�^���X
};

