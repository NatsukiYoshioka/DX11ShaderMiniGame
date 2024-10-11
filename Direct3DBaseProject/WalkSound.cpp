#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include "WalkSound.h"

WalkSound::WalkSound():
	m_volume(float(Json::GetInstance()->GetData()["WalkSoundVolume"]))
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["WalkSound"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

WalkSound::~WalkSound()
{

}

void WalkSound::InitializeTitle()
{
	Initialize();
}

void WalkSound::UpdateTitle()
{

}

void WalkSound::Initialize()
{
	m_soundInstance->Stop();
	m_soundInstance->SetVolume(m_volume);
}

void WalkSound::Update()
{
	auto animationState = PlayerAccessor::GetInstance()->GetPlayer()->GetNowAnimationState();
	if (animationState == Player::AnimationState::CrouchedWalk || animationState == Player::AnimationState::Walk)
	{
		m_soundInstance->Play(true);
	}
	else
	{
		m_soundInstance->Stop();
	}
}

void WalkSound::InitializeResult()
{
	Initialize();
}

void WalkSound::UpdateResult()
{

}