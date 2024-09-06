#pragma once
using namespace std;

class UIBase;

/// <summary>
/// UI用アクセサクラス
/// </summary>
class UIAccessor
{
private:
	/// <summary>
	/// UIの生成
	/// </summary>
	UIAccessor();

	/// <summary>
	/// データ破棄
	/// </summary>
	~UIAccessor();
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
	static UIAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// UIの取得
	/// </summary>
	/// <returns></returns>
	vector<UIBase*> GetUIs() { return m_UIs; }
private:
	static UIAccessor* m_instance;		//アクセサのインスタンス
	vector<UIBase*> m_UIs;				//UIのインスタンス配列
};

