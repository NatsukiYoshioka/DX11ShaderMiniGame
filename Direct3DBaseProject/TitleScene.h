#pragma once
#include"BaseScene.h"

class BaseScene;

/// <summary>
/// タイトルシーン管理クラス
/// </summary>
class TitleScene:public BaseScene
{
public:
	/// <summary>
	/// タイトルシーン初期化
	/// </summary>
	TitleScene();

	/// <summary>
	/// データ破棄
	/// </summary>
	~TitleScene();

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
	
	/// <summary>
	/// ゲームをスタートさせるかどうか取得
	/// </summary>
	bool GetIsStartGame()const { return m_isStartGame; }
private:
	bool m_isStartGame;		//ゲームをスタートさせるかどうか
};

