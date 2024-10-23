#pragma once
#include"GameObject.h"

using namespace DirectX;
using namespace std;

class GameObject;

/// <summary>
/// UIの基底クラス
/// </summary>
class SoundBase abstract:public GameObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SoundBase() {}

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SoundBase() {}

	//純粋仮想関数
	virtual void InitializeTitle() abstract;
	virtual void UpdateTitle() abstract;
	void DrawTitle()override{}
	virtual void Initialize() abstract;
	virtual void Update() abstract;
	void Draw()override{}
	virtual void InitializeResult() abstract;
	virtual void UpdateResult() abstract;
	void DrawResult()override{}
	void DrawShadow()override{}

	/// <summary>
	/// サウンドの状態取得
	/// </summary>
	SoundState GetSoundState()const { return m_soundInstance->GetState(); }

protected:
	unique_ptr<SoundEffect> m_sound;					//サウンドエフェクトクラス
	unique_ptr<SoundEffectInstance> m_soundInstance;	//サウンドエフェクトのインスタンスクラス
};