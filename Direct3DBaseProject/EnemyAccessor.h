#pragma once

class Enemy;

/// <summary>
/// �G�l�~�[�A�N�Z�T�N���X
/// </summary>
class EnemyAccessor
{
private:
	/// <summary>
	/// ���f���̐���
	/// </summary>
	EnemyAccessor();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~EnemyAccessor();
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
	static EnemyAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// �G�̃I�u�W�F�N�g�擾
	/// </summary>
	/// <returns>Enemy�I�u�W�F�N�g</returns>
	Enemy* GetEnemy() { return m_enemy; }
private:
	static EnemyAccessor* m_instance;	//�A�N�Z�T�̃C���X�^���X
	Enemy* m_enemy;						//�G�I�u�W�F�N�g�̃C���X�^���X
};

