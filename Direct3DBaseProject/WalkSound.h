#pragma once
#include"SoundBase.h"
#include"GameObject.h"

class SoundBase;

/// <summary>
/// 歩き足音管理クラス
/// </summary>
class WalkSound:public SoundBase
{
public:
	/// <summary>
	/// 歩いた時のSE初期化
	/// </summary>
	WalkSound();

	/// <summary>
	/// データ破棄
	/// </summary>
	~WalkSound();

	/// <summary>
	/// タイトルサウンド初期化
	/// </summary>
	void InitializeTitle()override;

	/// <summary>
	/// タイトルサウンド更新
	/// </summary>
	void UpdateTitle()override;

	/// <summary>
	/// ゲームシーンサウンド初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// ゲームシーンサウンド更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// リザルトサウンド初期化
	/// </summary>
	void InitializeResult()override;

	/// <summary>
	/// リザルトサウンド更新
	/// </summary>
	void UpdateResult()override;

private:
	const float m_volume;		//サウンドのボリューム
};

