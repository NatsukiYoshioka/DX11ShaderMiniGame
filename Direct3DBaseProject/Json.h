#pragma once
#include"json.hpp"

using json = nlohmann::json;
using namespace std;

/// <summary>
/// Jsonデータ管理クラス
/// </summary>
class Json
{
private:
	/// <summary>
	/// データのロード
	/// </summary>
	Json();

	/// <summary>
	/// データ破棄
	/// </summary>
	~Json();
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
	static Json* GetInstance() { return m_instance; }

	/// <summary>
	/// Jsonデータ取得
	/// </summary>
	/// <returns></returns>
	json GetData() { return m_json; }

	/// <summary>
	/// string型のwstring型変換
	/// </summary>
	/// <param name="str">変換する文字列</param>
	/// <returns>変換した文字列</returns>
	wstring Widen(const string& str);
private:
	static Json* m_instance;		//クラスのインスタンス

	static const string m_filePath;	//Jsonファイルのパス
	json m_json;					//Jsonのデータ
};

