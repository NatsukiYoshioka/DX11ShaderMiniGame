#pragma once

class DollHead;

/// <summary>
/// 人形の頭のアクセサ
/// </summary>
class DollHeadAccessor
{
private:
	/// <summary>
	/// 人形の頭オブジェクトのコンストラクタ
	/// </summary>
	DollHeadAccessor();

	/// <summary>
	/// 人形の頭オブジェクトのデストラクタ
	/// </summary>
	~DollHeadAccessor();
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
	static DollHeadAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// オブジェクトの取得
	/// </summary>
	DollHead* GetDollHead() { return m_dollHead; }
private:
	static DollHeadAccessor* m_instance;	//アクセサのインスタンス
	DollHead* m_dollHead;					//オブジェクトのインスタンス
};

