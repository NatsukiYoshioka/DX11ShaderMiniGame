#include "pch.h"
#include"DeviceAccessor.h"
#include"Json.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"GameObjectManager.h"
#include"UIBase.h"
#include "GameClearUI.h"

GameClearUI::GameClearUI():
	m_scale(float(Json::GetInstance()->GetData()["GameClearUIScale"])),
	m_layerDepth(float(Json::GetInstance()->GetData()["GameClearUILayerDepth"]))
{
	auto deviceAccessor = DeviceAccessor::GetInstance();
	auto json = Json::GetInstance();

	//テクスチャのロード
	SetCurrentDirectory(L"Assets/UI");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["GameClearUI"]).c_str(),
		m_textureResource.GetAddressOf(),
		m_texture.ReleaseAndGetAddressOf()));
	SetCurrentDirectory(L"../../");

	ComPtr<ID3D11Texture2D> tex;
	CD3D11_TEXTURE2D_DESC texDesc;
	DX::ThrowIfFailed(m_textureResource.As(&tex));
	tex->GetDesc(&texDesc);
	m_origin.x = float(texDesc.Width / 2);
	m_origin.y = float(texDesc.Height / 2);

	m_pos.x = deviceAccessor->GetScreenSize().right / 2;
	m_pos.y = float(json->GetData()["GameClearUIPosY"]);
}

GameClearUI::~GameClearUI()
{

}

//タイトルシーンオブジェクトの初期化
void GameClearUI::InitializeTitle()
{

}

//タイトルシーンオブジェクトの更新
void GameClearUI::UpdateTitle()
{

}

//タイトルシーンオブジェクトの描画
void GameClearUI::DrawTitle()
{

}

//UI初期化
void GameClearUI::Initialize()
{

}

//UI更新
void GameClearUI::Update()
{

}

//UI描画
void GameClearUI::Draw()
{

}

//リザルトシーンオブジェクトの初期化
void GameClearUI::InitializeResult()
{

}

//リザルトシーンオブジェクトの更新
void GameClearUI::UpdateResult()
{
	
}

//リザルトシーンオブジェクトの描画
void GameClearUI::DrawResult()
{
	if (!PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())return;
	auto batch = GameObjectManager::GetInstance()->GetSpriteBatch();

	batch->Draw(m_texture.Get(), m_pos, nullptr, Colors::White, 0, m_origin, m_scale, SpriteEffects_None, m_layerDepth);
}