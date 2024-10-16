#pragma once
#include"SoundBase.h"
#include"GameObject.h"

class SoundBase;

class GameBGM:public SoundBase
{
public:
	/// <summary>
	/// ゲームシーンBGMの初期化
	/// </summary>
	GameBGM();

	/// <summary>
	/// データ破棄
	/// </summary>
	~GameBGM();

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
	float m_volume;					//サウンドのボリューム
	const float m_initializeVolume;	//サウンドの初期ボリューム
};

