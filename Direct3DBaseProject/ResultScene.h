#pragma once

class BaseScene;

/// <summary>
/// リザルトシーン管理クラス
/// </summary>
class ResultScene : public BaseScene
{
public:
	/// <summary>
	/// シーン初期化
	/// </summary>
	ResultScene();

	/// <summary>
	/// データ破棄
	/// </summary>
	~ResultScene();

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

