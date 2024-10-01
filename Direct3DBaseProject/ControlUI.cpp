#include "pch.h"
#include"DeviceAccessor.h"
#include"Json.h"
#include"GameObjectManager.h"
#include"UIBase.h"
#include "ControlUI.h"

ControlUI::ControlUI():
	m_scale(float(Json::GetInstance()->GetData()["ControlUIScale"])),
	m_layerDepth(float(Json::GetInstance()->GetData()["ControlUILayerDepth"])),
	m_pos(Vector2(
		Json::GetInstance()->GetData()["ControlUIPosition"].at(0),
		Json::GetInstance()->GetData()["ControlUIPosition"].at(1))
	)
{
	auto deviceAccessor = DeviceAccessor::GetInstance();
	auto json = Json::GetInstance();

	//テクスチャのロード
	SetCurrentDirectory(L"Assets/UI");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["ControlUI"]).c_str(),
		m_textureResource.GetAddressOf(),
		m_texture.ReleaseAndGetAddressOf()));
	SetCurrentDirectory(L"../../");

	ComPtr<ID3D11Texture2D> tex;
	CD3D11_TEXTURE2D_DESC texDesc;
	DX::ThrowIfFailed(m_textureResource.As(&tex));
	tex->GetDesc(&texDesc);
	m_origin.x = float(texDesc.Width / 2);
	m_origin.y = float(texDesc.Height / 2);
}

ControlUI::~ControlUI()
{

}

//タイトルシーンオブジェクトの初期化
void ControlUI::InitializeTitle()
{

}

//タイトルシーンオブジェクトの更新
void ControlUI::UpdateTitle()
{

}

//タイトルシーンオブジェクトの描画
void ControlUI::DrawTitle()
{

}

//UI初期化
void ControlUI::Initialize()
{

}

//UI更新
void ControlUI::Update()
{

}

//UI描画
void ControlUI::Draw()
{
	auto batch = GameObjectManager::GetInstance()->GetSpriteBatch();

	batch->Draw(m_texture.Get(), m_pos, nullptr, Colors::White, 0, m_origin, m_scale, SpriteEffects_None, m_layerDepth);
}

//リザルトシーンオブジェクトの初期化
void ControlUI::InitializeResult()
{

}

//リザルトシーンオブジェクトの更新
void ControlUI::UpdateResult()
{

}

//リザルトシーンオブジェクトの描画
void ControlUI::DrawResult()
{
	
}