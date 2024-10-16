#pragma once
using namespace std;
using namespace DirectX::SimpleMath;

class Block;

/// <summary>
/// �ςݖ؃u���b�N�̃A�N�Z�T
/// </summary>
class BlockAccessor
{
private:
	/// <summary>
	/// �u���b�N�̐���
	/// </summary>
	BlockAccessor();

	/// <summary>
	/// �u���b�N�̔j��
	/// </summary>
	~BlockAccessor();
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
	/// <returns>�N���X�̃C���X�^���X</returns>
	static BlockAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// �u���b�N�̃R���e�i�擾
	/// </summary>
	/// <returns>�u���b�N�̃R���e�i</returns>
	vector<Block*> GetBlocks() { return m_blocks; }

private:
	static BlockAccessor* m_instance;	//�N���X�̃C���X�^���X

	vector<unique_ptr<Model>> m_model;	//�u���b�N�̃R���e�i
	vector<Block*> m_blocks;			//�u���b�N�̃R���e�i
};

