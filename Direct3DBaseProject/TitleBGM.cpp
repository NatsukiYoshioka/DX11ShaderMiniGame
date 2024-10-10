#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include "TitleBGM.h"

TitleBGM::TitleBGM():
	m_volume(1.f)
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["TitleBGM"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

TitleBGM::~TitleBGM()
{
	m_soundInstance.reset();
	m_sound.reset();
}

void TitleBGM::InitializeTitle()
{
	Initialize();
}

void TitleBGM::UpdateTitle()
{
	m_volume = CameraAccessor::GetInstance()->GetCamera()->GetTitlePosRatio();
	m_soundInstance->SetVolume(m_volume);
	m_soundInstance->Play(true);
}

void TitleBGM::Initialize()
{
	m_soundInstance->Stop();
}

void TitleBGM::Update()
{

}

void TitleBGM::InitializeResult()
{
	Initialize();
	m_volume = 1.f;
}

void TitleBGM::UpdateResult()
{

}