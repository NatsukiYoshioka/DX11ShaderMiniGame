#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"Transition.h"
#include"UIAccessor.h"
#include"BaseScene.h"
#include"SceneManager.h"
#include "GameClearBGM.h"

GameClearBGM::GameClearBGM() :
	m_volume(1.f)
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["GameClearBGM"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

GameClearBGM::~GameClearBGM()
{

}

void GameClearBGM::InitializeTitle()
{
	Initialize();
}

void GameClearBGM::UpdateTitle()
{

}

void GameClearBGM::Initialize()
{
	m_soundInstance->Stop();
	m_soundInstance->SetVolume(m_volume);
	m_volume = 1.f;
}

void GameClearBGM::Update()
{
	
}

void GameClearBGM::InitializeResult()
{
	Initialize();
}

void GameClearBGM::UpdateResult()
{
	if (!PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())return;
	auto isChangeScene = SceneManager::GetInstance()->GetNowScene()->GetIsChangeScene();
	for (int i = 0;i < UIAccessor::GetInstance()->GetUIs().size();i++)
	{
		auto transition = dynamic_cast<Transition*>(UIAccessor::GetInstance()->GetUIs().at(i));
		if (transition && isChangeScene)
		{
			m_volume = 1.f - transition->GetAlpha();
		}
	}
	m_soundInstance->SetVolume(m_volume);
	m_soundInstance->Play(false);
}
