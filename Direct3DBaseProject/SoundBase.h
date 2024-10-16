#pragma once
#include"GameObject.h"

using namespace DirectX;
using namespace std;

class GameObject;

/// <summary>
/// UIの基底クラス
/// </summary>
class SoundBase :public GameObject
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
	virtual void InitializeTitle() = 0;
	virtual void UpdateTitle() = 0;
	void DrawTitle()override{}
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	void Draw()override{}
	virtual void InitializeResult() = 0;
	virtual void UpdateResult() = 0;
	void DrawResult()override{}

	SoundState GetSoundState()const { return m_soundInstance->GetState(); }

protected:
	unique_ptr<SoundEffect> m_sound;					//サウンドエフェクトクラス
	unique_ptr<SoundEffectInstance> m_soundInstance;	//サウンドエフェクトのインスタンスクラス
};