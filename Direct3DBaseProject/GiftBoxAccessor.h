#pragma once

class GiftBox;

/// <summary>
/// �v���[���g���p�A�N�Z�T
/// </summary>
class GiftBoxAccessor
{
private:
	/// <summary>
	/// ���f���̐���
	/// </summary>
	GiftBoxAccessor();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~GiftBoxAccessor();
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
	static GiftBoxAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// �I�u�W�F�N�g�C���X�^���X�擾
	/// </summary>
	GiftBox* GetGiftBox() { return m_giftBox; }

private:
	static GiftBoxAccessor* m_instance;	//�A�N�Z�T�̃C���X�^���X
	GiftBox* m_giftBox;					//�I�u�W�F�N�g�̃C���X�^���X
};

