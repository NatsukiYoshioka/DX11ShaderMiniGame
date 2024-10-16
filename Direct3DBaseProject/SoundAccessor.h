#pragma once
using namespace std;

class SoundBase;

class SoundAccessor
{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SoundAccessor();

	/// <summary>
	/// データ破棄
	/// </summary>
	~SoundAccessor();

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
	static SoundAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// サウンドオブジェクトの取得
	/// </summary>
	/// <returns></returns>
	vector<SoundBase*> GetSounds() { return m_sounds; }
private:
	static SoundAccessor* m_instance;		//アクセサのインスタンス
	vector<SoundBase*> m_sounds;			//サウンドオブジェクトのコンテナ
};

