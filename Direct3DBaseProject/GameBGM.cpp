#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Transition.h"
#include"FoundUI.h"
#include"UIAccessor.h"
#include"FoundSE.h"
#include"SoundBase.h"
#include"SoundAccessor.h"
#include "GameBGM.h"

//ゲームシーンBGMの初期化
GameBGM::GameBGM():
	m_volume(0.f),
	m_initializeVolume(float(Json::GetInstance()->GetData()["GameBGMVolume"]))
{
	auto audioEngine = DeviceAccessor::GetInstance()->GetAudioEngine();
	auto json = Json::GetInstance();

	//サウンドのロード
	SetCurrentDirectory(L"Assets/Sounds");
	m_sound = make_unique<SoundEffect>(audioEngine, json->Widen(json->GetData()["GameBGM"]).c_str());
	m_soundInstance = m_sound->CreateInstance();
	SetCurrentDirectory(L"../../");
}

//データ破棄
GameBGM::~GameBGM()
{
	m_soundInstance.reset();
	m_sound.reset();
}

//タイトルでのサウンド初期化
void GameBGM::InitializeTitle()
{
	Initialize();
}

//タイトルでのサウンド更新(処理なし)
void GameBGM::UpdateTitle()
{
	
}

//ゲームシーンでのサウンド初期化
void GameBGM::Initialize()
{
	m_soundInstance->Stop();
	m_volume = m_initializeVolume;
}

//ゲームシーンでのサウンド更新
void GameBGM::Update()
{
	bool isFound = false;
	//完全に見つかったかどうかを取得
	for (int i = 0;i < UIAccessor::GetInstance()->GetUIs().size();i++)
	{
		auto foundUI = dynamic_cast<FoundUI*>(UIAccessor::GetInstance()->GetUIs().at(i));
		if (foundUI && foundUI->GetIsFound())
		{
			isFound = true;
			break;
		}
	}
	for (int i = 0;i < UIAccessor::GetInstance()->GetUIs().size();i++)
	{
		auto transition = dynamic_cast<Transition*>(UIAccessor::GetInstance()->GetUIs().at(i));
		//見つかったらトランジションのフェードインに合わせてボリュームを下げる
		if (transition && isFound)
		{
			m_volume = m_initializeVolume - transition->GetAlpha();
			if (m_volume <= 0.f)
			{
				m_volume = 0.f;
			}
			break;
		}
	}
	for (int i = 0;i < SoundAccessor::GetInstance()->GetSounds().size();i++)
	{
		auto foundSE = dynamic_cast<FoundSE*>(SoundAccessor::GetInstance()->GetSounds().at(i));
		//見つかっているときのSEが再生されているときはBGMのボリュームを半減させる
		if (foundSE)
		{
			if (foundSE->GetSoundState() == SoundState::PLAYING)
			{
				m_soundInstance->SetVolume(m_volume / 2);
			}
			else m_soundInstance->SetVolume(m_volume);

			break;
		}
	}
	m_soundInstance->Play(true);
}

//リザルトでのサウンド初期化
void GameBGM::InitializeResult()
{
	Initialize();
}

//リザルトでの更新(処理なし)
void GameBGM::UpdateResult()
{

}