#pragma once

class BaseScene;

/// <summary>
/// メインゲームシーン管理クラス
/// </summary>
class GameScene : public BaseScene
{
public:
	/// <summary>
	/// ゲームシーン初期化
	/// </summary>
	GameScene();

	/// <summary>
	/// データ破棄
	/// </summary>
	~GameScene();

	/// <summary>
	/// シーン更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// シーン描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// オフスクリーン描画
	/// </summary>
	void DrawOffScreen()override;
};

