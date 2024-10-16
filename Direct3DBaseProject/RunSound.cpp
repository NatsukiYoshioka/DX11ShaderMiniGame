#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include "RunSound.h"

//走った時の足音の初期化
RunSound::RunSound():
	m_volume(float(Json::GetInstance()->GetData()["RunSoundVolume"]))
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();

	//サウンドのロード
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["RunSound"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

//データ破棄
RunSound::~RunSound()
{

}

//タイトルでのサウンド初期化
void RunSound::InitializeTitle()
{
	Initialize();
}

//タイトルでのサウンド更新(処理なし)
void RunSound::UpdateTitle()
{

}

//ゲームシーンでのサウンド初期化
void RunSound::Initialize()
{
	m_soundInstance->Stop();
	m_soundInstance->SetVolume(m_volume);
}

//ゲームシーンでのサウンド更新
void RunSound::Update()
{
	auto animationState = PlayerAccessor::GetInstance()->GetPlayer()->GetNowAnimationState();
	//プレイヤーが走っていたら再生
	if (animationState == Player::AnimationState::Run)
	{
		m_soundInstance->Play(true);
	}
	else
	{
		m_soundInstance->Stop();
	}
}

//リザルトでのサウンド初期化
void RunSound::InitializeResult()
{
	Initialize();
}

//リザルトでのサウンド更新(処理なし)
void RunSound::UpdateResult()
{

}