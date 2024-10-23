#pragma once

class Room;

/// <summary>
/// 部屋モデル用アクセサ
/// </summary>
class RoomAccessor
{
private:
	/// <summary>
	/// モデルの生成
	/// </summary>
	RoomAccessor();

	/// <summary>
	/// モデルの破棄
	/// </summary>
	~RoomAccessor();
public:
	/// <summary>
	/// インスタンスの生成
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// インスタンスの破棄
	/// </summary>
	static void DestroyInstance();

	/// <summary>
	/// インスタンス取得
	/// </summary>
	static RoomAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// オブジェクトのインスタンス取得
	/// </summary>
	Room* GetRoom() { return m_room; }

private:
	static RoomAccessor* m_instance;	//アクセサのインスタンス
	Room* m_room;						//オブジェクトのインスタンス
};

