#include "pch.h"
#include"DeviceAccessor.h"
#include"Json.h"
#include"TitleLogo.h"
#include"UIBase.h"
#include"UIAccessor.h"
#include"GameObjectManager.h"
#include "ButtonA.h"

//AボタンUIの初期化
ButtonA::ButtonA():
	m_alpha(1.f),
	m_scale(float(Json::GetInstance()->GetData()["ButtonAScale"])),
	m_scaleAdd(float(Json::GetInstance()->GetData()["ButtonAScaleAdd"])),
	m_minScale(float(Json::GetInstance()->GetData()["ButtonAScale"])),
	m_maxScale(float(Json::GetInstance()->GetData()["ButtonAMaxScale"])),
	m_layerDepth(float(Json::GetInstance()->GetData()["ButtonALayerDepth"]))
{
	auto deviceAccessor = DeviceAccessor::GetInstance();
	auto json = Json::GetInstance();

	//テクスチャのロード
	SetCurrentDirectory(L"Assets/UI");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["ButtonA"]).c_str(),
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
	m_pos.y = float(json->GetData()["ButtonAPosition"].at(1));
}

//データ破棄
ButtonA::~ButtonA()
{

}

//タイトルシーンオブジェクトの初期化
void ButtonA::InitializeTitle()
{
	Initialize();
}

//タイトルシーンオブジェクトの更新
void ButtonA::UpdateTitle()
{
	for (int i = 0;i < UIAccessor::GetInstance()->GetUIs().size();i++)
	{
		auto titleLogo = dynamic_cast<TitleLogo*>(UIAccessor::GetInstance()->GetUIs().at(i));
		if (titleLogo)
		{
			m_alpha = titleLogo->GetAlpha();
		}
	}
	UpdateScale();
}

//タイトルシーンオブジェクトの描画
void ButtonA::DrawTitle()
{
	auto batch = GameObjectManager::GetInstance()->GetSpriteBatch();

	batch->Draw(m_texture.Get(), m_pos, nullptr, Colors::White * m_alpha, 0, m_origin, m_scale, SpriteEffects_None, m_layerDepth);
}

//UI初期化(処理なし)
void ButtonA::Initialize()
{
	m_alpha = 1.f;
}

//UI更新(処理なし)
void ButtonA::Update()
{

}

//UI描画(処理なし)
void ButtonA::Draw()
{
	
}

//リザルトシーンオブジェクトの初期化
void ButtonA::InitializeResult()
{
	Initialize();
}

//リザルトシーンオブジェクトの更新
void ButtonA::UpdateResult()
{
	UpdateScale();
}

//リザルトシーンオブジェクトの描画
void ButtonA::DrawResult()
{
	auto batch = GameObjectManager::GetInstance()->GetSpriteBatch();

	batch->Draw(m_texture.Get(), m_pos, nullptr, Colors::White * m_alpha, 0, m_origin, m_scale, SpriteEffects_None, m_layerDepth);
}

//テクスチャのスケール更新
void ButtonA::UpdateScale()
{
	if (m_scale <= m_minScale||m_scale>=m_maxScale)
	{
		m_scaleAdd *= -1.f;
	}
	m_scale += m_scaleAdd;
}