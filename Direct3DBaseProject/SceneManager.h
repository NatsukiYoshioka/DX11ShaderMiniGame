#pragma once

class BaseScene;

/// <summary>
/// シーン管理クラス
/// </summary>
class SceneManager
{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneManager();
public:
	/// <summary>
	/// シーンの状態を管理
	/// </summary>
	enum class Scene
	{
		Title,								//タイトル画面
		Game,								//ゲーム画面
		Result								//リザルト画面
	};

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
	static SceneManager* GetInstance() { return m_instance; }

	/// <summary>
	/// シーン遷移処理
	/// </summary>
	static void ChangeScene(Scene scene);

	/// <summary>
	/// シーン更新
	/// </summary>
	static void Update();

	/// <summary>
	/// シーン描画
	/// </summary>
	static void Draw();

	/// <summary>
	/// オフスクリーン描画
	/// </summary>
	static void DrawOffScreen();

	/// <summary>
	/// 現在のシーンを取得
	/// </summary>
	static BaseScene* GetNowScene() { return m_nowScene; }

private:
	static SceneManager* m_instance;		//マネージャーのインスタンス
	static BaseScene* m_nowScene;			//シーンのインスタンス
};

