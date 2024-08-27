#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"GameObjectManager.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"UIBase.h"
#include "FoundUI.h"

FoundUI::FoundUI()
{
	auto json = Json::GetInstance();
	auto deviceAccessor = DeviceAccessor::GetInstance();

	SetCurrentDirectory(L"Assets/UI");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["EyeBaseUI"]).c_str(),
		m_baseTextureResource.GetAddressOf(),
		m_baseTexture.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["EyeFoundUI"]).c_str(),
		m_foundTextureResource.GetAddressOf(),
		m_foundTexture.ReleaseAndGetAddressOf()));
	SetCurrentDirectory(L"../../");

	m_baseTexturePos = Vector2(float(json->GetData()["EyeBaseUIPosition"].at(0)), float(json->GetData()["EyeBaseUIPosition"].at(1)));
	m_foundTexturePos = Vector2(float(json->GetData()["EyeFoundUIPosition"].at(0)), float(json->GetData()["EyeFoundUIPosition"].at(1)));

	m_baseTextureRotation = float(json->GetData()["EyeBaseUIRotation"]);
	m_foundTextureRotation = float(json->GetData()["EyeFoundUIRotation"]);

	m_baseTextureScale = Vector2(float(json->GetData()["EyeBaseUIScale"].at(0)), float(json->GetData()["EyeBaseUIScale"].at(1)));
	m_foundTextureScale = Vector2(float(json->GetData()["EyeFoundUIScale"].at(0)), float(json->GetData()["EyeFoundUIScale"].at(1)));

	m_baseTextureDepth = float(json->GetData()["EyeBaseUIDepth"]);
	m_foundTextureDepth = float(json->GetData()["EyeFoundUIDepth"]);

	ComPtr<ID3D11Texture2D> tex;
	CD3D11_TEXTURE2D_DESC texDesc;
	DX::ThrowIfFailed(m_baseTextureResource.As(&tex));
	tex->GetDesc(&texDesc);
	m_baseTextureOrigin.x = float(texDesc.Width / 2);
	m_baseTextureOrigin.y = float(texDesc.Height / 2);

	DX::ThrowIfFailed(m_foundTextureResource.As(&tex));
	tex->GetDesc(&texDesc);
	m_foundTextureOrigin.x = float(texDesc.Width / 2);
	m_foundTextureOrigin.y = float(texDesc.Height / 2);
}

FoundUI::~FoundUI()
{

}

void FoundUI::Update()
{
	
}

void FoundUI::Draw()
{
	auto batch = GameObjectManager::GetInstance()->GetSpriteBatch();
	
	batch->Draw(
		m_baseTexture.Get(),
		m_baseTexturePos,
		nullptr,
		Colors::White,
		m_baseTextureRotation,
		m_baseTextureOrigin,
		m_baseTextureScale,
		SpriteEffects_None,
		m_baseTextureDepth
	);
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetBeFound())
	{
		batch->Draw(
			m_foundTexture.Get(),
			m_foundTexturePos,
			nullptr,
			Colors::White,
			m_foundTextureRotation,
			m_foundTextureOrigin,
			m_foundTextureScale,
			SpriteEffects_None,
			m_foundTextureDepth
		);
	}
}