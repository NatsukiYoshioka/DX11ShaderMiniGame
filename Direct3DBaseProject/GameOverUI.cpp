#include "pch.h"
#include"DeviceAccessor.h"
#include"Json.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"DollHead.h"
#include"DollHeadAccessor.h"
#include"GameObjectManager.h"
#include"UIBase.h"
#include "GameOverUI.h"

//ゲームオーバー時UIの初期化
GameOverUI::GameOverUI():
	m_alpha(0),
	m_scale(float(Json::GetInstance()->GetData()["GameOverUIScale"])),
	m_layerDepth(float(Json::GetInstance()->GetData()["GameOverUILayerDepth"])),
	m_pos()
{
	auto deviceAccessor = DeviceAccessor::GetInstance();
	auto json = Json::GetInstance();

	//テクスチャのロード
	SetCurrentDirectory(L"Assets/UI");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["GameOverUI"]).c_str(),
		m_textureResource.GetAddressOf(),
		m_texture.ReleaseAndGetAddressOf()));
	SetCurrentDirectory(L"../../");

	ComPtr<ID3D11Texture2D> tex;
	CD3D11_TEXTURE2D_DESC texDesc;
	DX::ThrowIfFailed(m_textureResource.As(&tex));
	tex->GetDesc(&texDesc);
	m_origin.x = float(texDesc.Width / 2);
	m_origin.y = float(texDesc.Height / 2);

	m_pos.x = (float)deviceAccessor->GetScreenSize().right / 2;
	m_pos.y = float(json->GetData()["GameOverUIPosY"]);
}

//データ破棄
GameOverUI::~GameOverUI()
{

}

//タイトルシーンオブジェクトの初期化(処理なし)
void GameOverUI::InitializeTitle()
{

}

//タイトルシーンオブジェクトの更新(処理なし)
void GameOverUI::UpdateTitle()
{

}

//タイトルシーンオブジェクトの描画(処理なし)
void GameOverUI::DrawTitle()
{

}

//UI初期化(処理なし)
void GameOverUI::Initialize()
{

}

//UI更新(処理なし)
void GameOverUI::Update()
{
	
}

//UI描画(処理なし)
void GameOverUI::Draw()
{
	
}

//リザルトシーンオブジェクトの初期化(処理なし)
void GameOverUI::InitializeResult()
{

}

//リザルトシーンオブジェクトの更新
void GameOverUI::UpdateResult()
{
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())return;
	m_alpha = DollHeadAccessor::GetInstance()->GetDollHead()->GetPosRatio();
}

//リザルトシーンオブジェクトの描画
void GameOverUI::DrawResult()
{
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())return;
	auto batch = GameObjectManager::GetInstance()->GetSpriteBatch();

	batch->Draw(m_texture.Get(), m_pos, nullptr, Colors::White * m_alpha, 0, m_origin, m_scale, SpriteEffects_None, m_layerDepth);
}