#pragma once

class Desk;

/// <summary>
/// デスク3Dモデルのアクセサ
/// </summary>
class DeskAccessor
{
private:
	/// <summary>
	/// モデルの生成
	/// </summary>
	DeskAccessor();

	/// <summary>
	/// データ破棄
	/// </summary>
	~DeskAccessor();
public:
	/// <summary>
	/// アクセサのインスタンス生成
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// アクセサのインスタンスインスタンス破棄
	/// </summary>
	static void DestroyInstance();

	/// <summary>
	/// アクセサのインスタンス取得
	/// </summary>
	/// <returns>アクセサのインスタンス</returns>
	static DeskAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// デスクインスタンス取得
	/// </summary>
	/// <returns>デスクのインスタンス</returns>
	Desk* GetDesk() { return m_desk; }
private:
	static DeskAccessor* m_instance;	//アクセサのインスタンス
	Desk* m_desk;						//デスクのインスタンス
};

