#include "pch.h"
#include"DeviceAccessor.h"
#include"Json.h"
#include"TitleScene.h"
#include"BaseScene.h"
#include"SceneManager.h"
#include"GameObjectManager.h"
#include"UIBase.h"
#include "TitleLogo.h"

TitleLogo::TitleLogo():
	m_alpha(1.f),
	m_renderTime(0.f),
	m_scale(float(Json::GetInstance()->GetData()["TitleLogoScale"])),
	m_layerDepth(float(Json::GetInstance()->GetData()["TitleLogoLayerDepth"])),
	m_fadeoutTime(float(Json::GetInstance()->GetData()["TitleLogoRenderTime"]))
{
	auto deviceAccessor = DeviceAccessor::GetInstance();
	auto json = Json::GetInstance();

	//テクスチャのロード
	SetCurrentDirectory(L"Assets/UI");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["TitleLogo"]).c_str(),
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
	m_pos.y = deviceAccessor->GetScreenSize().bottom / 2;
}

TitleLogo::~TitleLogo()
{

}

//タイトルシーンオブジェクトの初期化
void TitleLogo::InitializeTitle()
{
	m_alpha = 1.f;
	m_renderTime = 0.f;
}

//タイトルシーンオブジェクトの更新
void TitleLogo::UpdateTitle()
{
	auto title = dynamic_cast<TitleScene*>(SceneManager::GetInstance()->GetNowScene());
	if (title && title->GetIsStartGame())
	{
		m_renderTime += *DeviceAccessor::GetInstance()->GetElapsedTime();
		m_alpha = 1.f - (1.f * (m_renderTime / m_fadeoutTime));
		if (m_alpha <= 0.f)
		{
			m_alpha = 0.f;
		}
	}
}

//タイトルシーンオブジェクトの描画
void TitleLogo::DrawTitle()
{
	auto batch = GameObjectManager::GetInstance()->GetSpriteBatch();

	batch->Draw(m_texture.Get(), m_pos, nullptr, Colors::White * m_alpha, 0, m_origin, m_scale, SpriteEffects_None, m_layerDepth);
}

//UI初期化
void TitleLogo::Initialize()
{

}

//UI更新
void TitleLogo::Update()
{

}

//UI描画
void TitleLogo::Draw()
{

}

//リザルトシーンオブジェクトの初期化
void TitleLogo::InitializeResult()
{

}

//リザルトシーンオブジェクトの更新
void TitleLogo::UpdateResult()
{
	
}

//リザルトシーンオブジェクトの描画
void TitleLogo::DrawResult()
{
	
}