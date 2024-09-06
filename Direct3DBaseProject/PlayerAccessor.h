#pragma once

class Player;

/// <summary>
/// プレイヤーのアクセサクラス
/// </summary>
class PlayerAccessor
{
private:
	/// <summary>
	/// モデルの生成
	/// </summary>
	PlayerAccessor();

	/// <summary>
	/// データ破棄
	/// </summary>
	~PlayerAccessor();
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
	static PlayerAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// プレイヤーオブジェクトの取得
	/// </summary>
	/// <returns>Playerオブジェクト</returns>
	Player* GetPlayer() { return m_player; }
private:
	static PlayerAccessor* m_instance;		//アクセサのインスタンス
	Player* m_player;						//プレイヤーオブジェクトクラス
};

