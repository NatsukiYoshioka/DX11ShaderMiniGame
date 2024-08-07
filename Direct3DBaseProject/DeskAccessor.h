#pragma once

class Desk;

/// <summary>
/// �f�X�N3D���f���̃A�N�Z�T
/// </summary>
class DeskAccessor
{
private:
	/// <summary>
	/// ���f���̐���
	/// </summary>
	DeskAccessor();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~DeskAccessor();
public:
	/// <summary>
	/// �A�N�Z�T�̃C���X�^���X����
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// �A�N�Z�T�̃C���X�^���X�C���X�^���X�j��
	/// </summary>
	static void DestroyInstance();

	/// <summary>
	/// �A�N�Z�T�̃C���X�^���X�擾
	/// </summary>
	/// <returns>�A�N�Z�T�̃C���X�^���X</returns>
	static DeskAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// �f�X�N�C���X�^���X�擾
	/// </summary>
	/// <returns>�f�X�N�̃C���X�^���X</returns>
	Desk* GetDesk() { return m_desk; }
private:
	static DeskAccessor* m_instance;	//�A�N�Z�T�̃C���X�^���X
	Desk* m_desk;						//�f�X�N�̃C���X�^���X
};

