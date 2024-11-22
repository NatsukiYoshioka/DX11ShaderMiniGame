#pragma once

class GoalObject;

class GoalObjectAccessor
{
private:
	/// <summary>
	/// オブジェクトの生成
	/// </summary>
	GoalObjectAccessor();

	/// <summary>
	/// オブジェクト破棄
	/// </summary>
	~GoalObjectAccessor();
public:
	/// <summary>
	/// インスタンスの生成
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// インスタンス破棄
	/// </summary>
	static void DestroyInstance();

	/// <summary>
	/// インスタンス取得
	/// </summary>
	static GoalObjectAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// オブジェクト取得
	/// </summary>
	GoalObject* GetGoalObject() { return m_goalObject; }
private:
	static GoalObjectAccessor* m_instance;		//アクセサのインスタンス
	GoalObject* m_goalObject;					//オブジェクトのインスタンス
};

