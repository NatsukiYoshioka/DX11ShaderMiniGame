#pragma once
using namespace std;
using namespace DirectX::SimpleMath;

class Block;

/// <summary>
/// 積み木ブロックのアクセサ
/// </summary>
class BlockAccessor
{
private:
	/// <summary>
	/// ブロックの生成
	/// </summary>
	BlockAccessor();

	/// <summary>
	/// ブロックの破棄
	/// </summary>
	~BlockAccessor();
public:
	/// <summary>
	/// インスタンス生成
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// インスタンス破棄
	/// </summary>
	static void DestroyInstance();

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>クラスのインスタンス</returns>
	static BlockAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// ブロックのコンテナ取得
	/// </summary>
	/// <returns>ブロックのコンテナ</returns>
	vector<Block*> GetBlocks() { return m_blocks; }

private:
	static BlockAccessor* m_instance;	//クラスのインスタンス

	vector<unique_ptr<Model>> m_model;	//ブロックのコンテナ
	vector<Block*> m_blocks;			//ブロックのコンテナ
};

