#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include "TitleBGM.h"

//タイトルBGMの初期化
TitleBGM::TitleBGM():
	m_volume(1.f)
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();

	//サウンドのロード
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["TitleBGM"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

//データ破棄
TitleBGM::~TitleBGM()
{
	m_soundInstance.reset();
	m_sound.reset();
}

//タイトルでのサウンド初期化
void TitleBGM::InitializeTitle()
{
	Initialize();
}

//タイトルでのサウンド更新
void TitleBGM::UpdateTitle()
{
	//タイトルでのカメラの移動割合でサウンドのボリュームを設定
	m_volume = CameraAccessor::GetInstance()->GetCamera()->GetTitlePosRatio();
	m_soundInstance->SetVolume(m_volume);
	m_soundInstance->Play(true);
}

//ゲームシーンでのサウンド初期化
void TitleBGM::Initialize()
{
	m_soundInstance->Stop();
	m_volume = 1.f;
}

//ゲームシーンでのサウンド更新(処理なし)
void TitleBGM::Update()
{

}

//リザルトでのサウンド初期化
void TitleBGM::InitializeResult()
{
	Initialize();
}

//リザルトでのサウンド更新
void TitleBGM::UpdateResult()
{

}