#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include "WalkSound.h"

//歩いた時のSE初期化
WalkSound::WalkSound():
	m_volume(float(Json::GetInstance()->GetData()["WalkSoundVolume"]))
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();

	//サウンドのロード
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["WalkSound"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

//データ破棄
WalkSound::~WalkSound()
{

}

//タイトルでのサウンド初期化
void WalkSound::InitializeTitle()
{
	Initialize();
}

//タイトルでのサウンド更新(処理なし)
void WalkSound::UpdateTitle()
{

}

//ゲームシーンでのサウンド初期化
void WalkSound::Initialize()
{
	m_soundInstance->Stop();
	m_soundInstance->SetVolume(m_volume);
}

//ゲームシーンでのサウンド更新
void WalkSound::Update()
{
	auto animationState = PlayerAccessor::GetInstance()->GetPlayer()->GetNowAnimationState();
	//プレイヤーが歩いていたらサウンド再生
	if (animationState == Player::AnimationState::CrouchedWalk || animationState == Player::AnimationState::Walk)
	{
		m_soundInstance->Play(true);
	}
	else
	{
		m_soundInstance->Stop();
	}
}

//リザルトでのサウンド初期化
void WalkSound::InitializeResult()
{
	Initialize();
}

//リザルトでのサウンド更新(処理なし)
void WalkSound::UpdateResult()
{

}