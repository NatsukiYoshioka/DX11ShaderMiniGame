#pragma once
using namespace std;
using namespace DirectX::SimpleMath;

class Block;

class BlockAccessor
{
private:
	BlockAccessor();
	~BlockAccessor();
public:
	static void CreateInstance();
	static void DestroyInstance();
	static BlockAccessor* GetInstance() { return m_instance; }

	vector<Block*> GetBlocks() { return m_blocks; }

private:
	static BlockAccessor* m_instance;

	vector<Block*> m_blocks;
};

