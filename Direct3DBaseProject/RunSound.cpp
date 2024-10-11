#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include "RunSound.h"

RunSound::RunSound():
	m_volume(float(Json::GetInstance()->GetData()["RunSoundVolume"]))
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["RunSound"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

RunSound::~RunSound()
{

}

void RunSound::InitializeTitle()
{
	Initialize();
}

void RunSound::UpdateTitle()
{

}

void RunSound::Initialize()
{
	m_soundInstance->Stop();
	m_soundInstance->SetVolume(m_volume);
}

void RunSound::Update()
{
	auto animationState = PlayerAccessor::GetInstance()->GetPlayer()->GetNowAnimationState();
	if (animationState == Player::AnimationState::Run)
	{
		m_soundInstance->Play(true);
	}
	else
	{
		m_soundInstance->Stop();
	}
}

void RunSound::InitializeResult()
{
	Initialize();
}

void RunSound::UpdateResult()
{

}