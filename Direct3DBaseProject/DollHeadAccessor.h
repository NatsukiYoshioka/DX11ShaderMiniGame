#pragma once

class DollHead;

class DollHeadAccessor
{
private:
	/// <summary>
	/// �l�`�̓��I�u�W�F�N�g�̃R���X�g���N�^
	/// </summary>
	DollHeadAccessor();

	/// <summary>
	/// �l�`�̓��I�u�W�F�N�g�̃f�X�g���N�^
	/// </summary>
	~DollHeadAccessor();
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
	/// <returns></returns>
	static DollHeadAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// �I�u�W�F�N�g�̎擾
	/// </summary>
	/// <returns></returns>
	DollHead* GetDollHead() { return m_dollHead; }
private:
	static DollHeadAccessor* m_instance;	//�A�N�Z�T�̃C���X�^���X
	DollHead* m_dollHead;					//�I�u�W�F�N�g�̃C���X�^���X
};

