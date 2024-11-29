#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"FoundUI.h"
#include"UIBase.h"
#include"UIAccessor.h"
#include "FoundSE.h"

//サウンドの初期化
FoundSE::FoundSE():
	m_pitch(0.f),
	m_volume(0.f),
	m_maxVolume(float(Json::GetInstance()->GetData()["FoundSEVolume"]))
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();

	//サウンドのロード
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["FoundSE"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

//データ破棄
FoundSE::~FoundSE()
{

}

//タイトルでの初期化
void FoundSE::InitializeTitle()
{
	Initialize();
}

//タイトルでの更新(処理なし)
void FoundSE::UpdateTitle()
{

}

//ゲームシーンでの初期化
void FoundSE::Initialize()
{
	m_soundInstance->Stop();
	m_soundInstance->SetVolume(m_volume);
}

//ゲームシーンでの更新
void FoundSE::Update()
{
	m_soundInstance->Play(true);
	//見つかっていたらピッチを上げながら再生
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetBeFound() &&
		PlayerAccessor::GetInstance()->GetPlayer()->GetNowAnimationState() != Player::AnimationState::Die&&
		!PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())
	{
		for (int i = 0; i < UIAccessor::GetInstance()->GetUIs().size(); i++)
		{
			auto foundUI = dynamic_cast<FoundUI*>(UIAccessor::GetInstance()->GetUIs().at(i));
			if (foundUI)
			{
				m_pitch = 1.f * foundUI->GetTimeRatio();
				m_soundInstance->SetPitch(m_pitch);
			}
		}
		m_volume += 0.01f;
		if (m_volume >= m_maxVolume)
		{
			m_volume = m_maxVolume;
		}
		m_soundInstance->SetVolume(m_volume);
	}
	else
	{
		m_volume -= 0.1f;
		if (m_volume <= 0.f)
		{
			m_volume = 0.f;
			m_soundInstance->Stop(true);
		}
		m_soundInstance->SetVolume(m_volume);
		m_soundInstance->SetPitch(m_pitch);
	}
}

//リザルトでの初期化
void FoundSE::InitializeResult()
{
	Initialize();
}

//リザルトでの更新(処理なし)
void FoundSE::UpdateResult()
{

}