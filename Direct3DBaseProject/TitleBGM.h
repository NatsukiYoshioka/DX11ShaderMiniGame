#pragma once
#include"SoundBase.h"
#include"GameObject.h"

class SoundBase;

class TitleBGM:public SoundBase
{
public:
	/// <summary>
	/// タイトルBGMの初期化
	/// </summary>
	TitleBGM();

	/// <summary>
	/// データ破棄
	/// </summary>
	~TitleBGM();

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
	float m_volume;		//サウンドのボリューム
};

