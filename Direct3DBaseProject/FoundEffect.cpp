#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"GameObjectManager.h"
#include"FoundUI.h"
#include"UIBase.h"
#include"UIAccessor.h"
#include "FoundEffect.h"

//見つかった時のUIエフェクト初期化
FoundEffect::FoundEffect():
	m_alpha(0),
	m_scale(float(Json::GetInstance()->GetData()["FoundEffectScale"])),
	m_layerDepth(float(Json::GetInstance()->GetData()["FoundEffectLayerDepth"]))
{
	auto deviceAccessor = DeviceAccessor::GetInstance();
	auto json = Json::GetInstance();

	//テクスチャのロード
	SetCurrentDirectory(L"Assets/UI");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["FoundEffectTexture"]).c_str(),
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

//データ破棄
FoundEffect::~FoundEffect()
{

}

//タイトルシーンオブジェクトの初期化(処理なし)
void FoundEffect::InitializeTitle()
{

}

//タイトルシーンオブジェクトの更新(処理なし)
void FoundEffect::UpdateTitle()
{
	
}

//タイトルシーンオブジェクトの描画(処理なし)
void FoundEffect::DrawTitle()
{
	
}

//UI初期化(処理なし)
void FoundEffect::Initialize()
{

}

//UI更新
void FoundEffect::Update()
{
	for (int i = 0;i < UIAccessor::GetInstance()->GetUIs().size();i++)
	{
		auto foundUI = dynamic_cast<FoundUI*>(UIAccessor::GetInstance()->GetUIs().at(i));
		if (foundUI)
		{
			m_alpha = foundUI->GetTimeRatio();
			break;
		}
	}
}

//UI描画
void FoundEffect::Draw()
{
	auto batch = GameObjectManager::GetInstance()->GetSpriteBatch();
	auto size = DeviceAccessor::GetInstance()->GetScreenSize();
	auto pos = Vector2(size.right/2, size.bottom/2);
	
	batch->Draw(m_texture.Get(), pos, nullptr, Colors::White * m_alpha, 0, m_origin, m_scale, SpriteEffects_None, m_layerDepth);
}

//リザルトシーンオブジェクトの初期化(処理なし)
void FoundEffect::InitializeResult()
{

}

//リザルトシーンオブジェクトの更新(処理なし)
void FoundEffect::UpdateResult()
{
	
}

//リザルトシーンオブジェクトの描画(処理なし)
void FoundEffect::DrawResult()
{
	
}