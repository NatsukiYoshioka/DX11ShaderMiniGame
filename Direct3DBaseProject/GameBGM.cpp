#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Transition.h"
#include"FoundUI.h"
#include"UIAccessor.h"
#include "GameBGM.h"

GameBGM::GameBGM():
	m_volume(0.f),
	m_initializeVolume(float(Json::GetInstance()->GetData()["GameBGMVolume"]))
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["GameBGM"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

GameBGM::~GameBGM()
{
	m_soundInstance.reset();
	m_sound.reset();
}

void GameBGM::InitializeTitle()
{
	Initialize();
}

void GameBGM::UpdateTitle()
{
	
}

void GameBGM::Initialize()
{
	m_soundInstance->Stop();
	m_volume = m_initializeVolume;
}

void GameBGM::Update()
{
	bool isFound = false;
	for (int i = 0;i < UIAccessor::GetInstance()->GetUIs().size();i++)
	{
		auto foundUI = dynamic_cast<FoundUI*>(UIAccessor::GetInstance()->GetUIs().at(i));
		if (foundUI && foundUI->GetIsFound())
		{
			isFound = true;
		}
	}
	for (int i = 0;i < UIAccessor::GetInstance()->GetUIs().size();i++)
	{
		auto transition = dynamic_cast<Transition*>(UIAccessor::GetInstance()->GetUIs().at(i));
		if (transition && isFound)
		{
			m_volume = m_initializeVolume - transition->GetAlpha();
			if (m_volume <= 0.f)
			{
				m_volume = 0.f;
			}
		}
	}
	m_soundInstance->SetVolume(m_volume);
	m_soundInstance->Play(true);
}

void GameBGM::InitializeResult()
{
	Initialize();
}

void GameBGM::UpdateResult()
{

}