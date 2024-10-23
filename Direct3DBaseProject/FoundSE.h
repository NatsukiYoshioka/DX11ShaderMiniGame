#pragma once
#include"SoundBase.h"
#include"GameObject.h"

class SoundBase;

/// <summary>
/// 見つかった時のサウンドエフェクト管理クラス
/// </summary>
class FoundSE:public SoundBase
{
public:
	/// <summary>
	/// サウンドの初期化
	/// </summary>
	FoundSE();

	/// <summary>
	/// データ破棄
	/// </summary>
	~FoundSE();

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
	float m_pitch;			//サウンドのピッチ
	const float m_volume;	//サウンドのボリューム
};

