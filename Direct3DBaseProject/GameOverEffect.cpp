#include "pch.h"
#include"DeviceAccessor.h"
#include"Json.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"DollHead.h"
#include"DollHeadAccessor.h"
#include"GameObjectManager.h"
#include"UIBase.h"
#include "GameOverEffect.h"

//ゲームオーバー時UIエフェクトの初期化
GameOverEffect::GameOverEffect():
	m_pos(),
	m_alpha(1.f),
	m_scale(float(Json::GetInstance()->GetData()["GameOverEffectScale"])),
	m_layerDepth(float(Json::GetInstance()->GetData()["GameOverEffectLayerDepth"]))
{
	auto deviceAccessor = DeviceAccessor::GetInstance();
	auto json = Json::GetInstance();

	//テクスチャのロード
	SetCurrentDirectory(L"Assets/UI");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["GameOverEffect"]).c_str(),
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
	m_pos.y = (float)deviceAccessor->GetScreenSize().bottom / 2;
}

//データ破棄
GameOverEffect::~GameOverEffect()
{

}

//タイトルシーンオブジェクトの初期化(処理なし)
void GameOverEffect::InitializeTitle()
{

}

//タイトルシーンオブジェクトの更新(処理なし)
void GameOverEffect::UpdateTitle()
{

}

//タイトルシーンオブジェクトの描画(処理なし)
void GameOverEffect::DrawTitle()
{

}

//UI初期化(処理なし)
void GameOverEffect::Initialize()
{

}

//UI更新(処理なし)
void GameOverEffect::Update()
{

}

//UI描画(処理なし)
void GameOverEffect::Draw()
{

}

//リザルトシーンオブジェクトの初期化(処理なし)
void GameOverEffect::InitializeResult()
{

}

//リザルトシーンオブジェクトの更新
void GameOverEffect::UpdateResult()
{
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())return;
	//人形の頭の移動割合に合わせてフェードイン
	m_alpha = DollHeadAccessor::GetInstance()->GetDollHead()->GetPosRatio();
}

//リザルトシーンオブジェクトの描画
void GameOverEffect::DrawResult()
{
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())return;
	auto batch = GameObjectManager::GetInstance()->GetSpriteBatch();

	batch->Draw(m_texture.Get(), m_pos, nullptr, Colors::White * m_alpha, 0, m_origin, m_scale, SpriteEffects_None, m_layerDepth);
}