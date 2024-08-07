#pragma once
using namespace std;

class GameObject;

/// <summary>
/// ゲームオブジェクト管理クラス
/// </summary>
class GameObjectManager
{
private:
	/// <summary>
	/// 管理するオブジェクトの設定
	/// </summary>
	GameObjectManager();

	/// <summary>
	/// データ破棄
	/// </summary>
	~GameObjectManager();
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
	static GameObjectManager* GetInstance() { return m_instance; }

	/// <summary>
	/// オブジェクトの更新
	/// </summary>
	void Update();

	/// <summary>
	/// オブジェクトの描画
	/// </summary>
	void Draw();

private:
	static GameObjectManager* m_instance;	//マネージャのインスタンス

	vector<GameObject*> m_gameObjects;		//ゲームオブジェクトのコンテナ
};

