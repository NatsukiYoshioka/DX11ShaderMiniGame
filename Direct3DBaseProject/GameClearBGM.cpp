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

//クリア時BGMの初期化
GameClearBGM::GameClearBGM() :
	m_volume(1.f)
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();

	//サウンドのロード
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["GameClearBGM"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

//データ破棄
GameClearBGM::~GameClearBGM()
{

}

//タイトルでのサウンド初期化
void GameClearBGM::InitializeTitle()
{
	Initialize();
}

//タイトルでのサウンド更新(処理なし)
void GameClearBGM::UpdateTitle()
{

}

//ゲームシーンでのサウンド初期化
void GameClearBGM::Initialize()
{
	m_soundInstance->Stop();
	m_soundInstance->SetVolume(m_volume);
	m_volume = 1.f;
}

//ゲームシーンでのサウンド更新(処理なし)
void GameClearBGM::Update()
{
	
}

//リザルトでのサウンド初期化
void GameClearBGM::InitializeResult()
{
	Initialize();
}

//リザルトでのサウンド更新
void GameClearBGM::UpdateResult()
{
	if (!PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())return;
	auto isChangeScene = SceneManager::GetInstance()->GetNowScene()->GetIsChangeScene();
	for (int i = 0;i < UIAccessor::GetInstance()->GetUIs().size();i++)
	{
		auto transition = dynamic_cast<Transition*>(UIAccessor::GetInstance()->GetUIs().at(i));
		//トランジションに合わせてサウンドのボリュームを減少させる
		if (transition && isChangeScene)
		{
			m_volume = 1.f - transition->GetAlpha();
		}
	}
	m_soundInstance->SetVolume(m_volume);
	m_soundInstance->Play(false);
}
