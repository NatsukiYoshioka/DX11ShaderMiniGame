#pragma once

class GiftBox;

class GiftBoxAccessor
{
private:
	/// <summary>
	/// モデルの生成
	/// </summary>
	GiftBoxAccessor();

	/// <summary>
	/// データ破棄
	/// </summary>
	~GiftBoxAccessor();
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
	/// <returns></returns>
	static GiftBoxAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// オブジェクトインスタンス取得
	/// </summary>
	/// <returns></returns>
	GiftBox* GetGiftBox() { return m_giftBox; }

private:
	static GiftBoxAccessor* m_instance;	//アクセサのインスタンス
	GiftBox* m_giftBox;					//オブジェクトのインスタンス
};

