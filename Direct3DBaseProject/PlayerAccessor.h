#pragma once

class Player;

/// <summary>
/// �v���C���[�̃A�N�Z�T�N���X
/// </summary>
class PlayerAccessor
{
private:
	/// <summary>
	/// ���f���̐���
	/// </summary>
	PlayerAccessor();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~PlayerAccessor();
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
	static PlayerAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// �v���C���[�I�u�W�F�N�g�̎擾
	/// </summary>
	/// <returns>Player�I�u�W�F�N�g</returns>
	Player* GetPlayer() { return m_player; }
private:
	static PlayerAccessor* m_instance;		//�A�N�Z�T�̃C���X�^���X
	Player* m_player;						//�v���C���[�I�u�W�F�N�g�N���X
};

