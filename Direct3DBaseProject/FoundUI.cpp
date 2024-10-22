#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"GameObjectManager.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"UIBase.h"
#include "FoundUI.h"

//UI初期化
FoundUI::FoundUI():
	m_timeRatio(0),
	m_foundTime(0),
	m_isFound(false),
	m_maxFoundTime(float(Json::GetInstance()->GetData()["MaxFoundTime"]))
{
	auto json = Json::GetInstance();
	auto deviceAccessor = DeviceAccessor::GetInstance();

	//テクスチャのロード
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

	//テクスチャ用の設定を初期化
	m_baseTexturePos = Vector2(float(json->GetData()["EyeBaseUIPosition"].at(0)), float(json->GetData()["EyeBaseUIPosition"].at(1)));
	m_foundTexturePos = Vector2(float(json->GetData()["EyeFoundUIPosition"].at(0)), float(json->GetData()["EyeFoundUIPosition"].at(1)));

	m_baseTextureRotation = float(json->GetData()["EyeBaseUIRotation"]) * XM_PI / 180.f;
	m_foundTextureRotation = float(json->GetData()["EyeFoundUIRotation"]) * XM_PI / 180.f;

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

	m_color = Colors::White;

	m_rectangle.left = 0;
	m_rectangle.right = texDesc.Width;
	m_rectangle.top = 0;
	m_rectangle.bottom = texDesc.Height;
	m_textureWidth = texDesc.Width;
}

//データ破棄
FoundUI::~FoundUI()
{
	m_baseTexture.Reset();
	m_baseTextureResource.Reset();
	m_foundTexture.Reset();
	m_foundTextureResource.Reset();
}

//タイトルシーンオブジェクトの初期化(処理なし)
void FoundUI::InitializeTitle()
{
	m_timeRatio = 0.f;
}

//タイトルシーンオブジェクトの更新(処理なし)
void FoundUI::UpdateTitle()
{

}

//タイトルシーンオブジェクトの描画(処理なし)
void FoundUI::DrawTitle()
{

}

//UI初期化
void FoundUI::Initialize()
{
	m_foundTime = 0;
	m_isFound = false;
}

//UI更新
void FoundUI::Update()
{
	auto elapsedTime = *DeviceAccessor::GetInstance()->GetElapsedTime();
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetBeFound() && !PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())
	{
		m_foundTime += elapsedTime;
	}
	else
	{
		m_foundTime -= elapsedTime;
	}
	if (m_foundTime < 0)
	{
		m_foundTime = 0;
	}
	if (m_foundTime >= m_maxFoundTime && !m_isFound)
	{
		m_foundTime = m_maxFoundTime;
		m_isFound = true;
	}
	if (m_isFound)
	{
		m_foundTime = m_maxFoundTime;
	}
	m_timeRatio = m_foundTime / m_maxFoundTime;
	m_rectangle.right = 0 + (m_textureWidth * m_timeRatio);
}

//UI描画
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
	batch->Draw(
		m_foundTexture.Get(),
		m_foundTexturePos,
		&m_rectangle,
		Colors::White,
		m_foundTextureRotation,
		m_foundTextureOrigin,
		m_foundTextureScale,
		SpriteEffects_None,
		m_foundTextureDepth
	);
}

//リザルトシーンオブジェクトの初期化(処理なし)
void FoundUI::InitializeResult()
{

}

//リザルトシーンオブジェクトの更新(処理なし)
void FoundUI::UpdateResult()
{

}

//リザルトシーンオブジェクトの描画(処理なし)
void FoundUI::DrawResult()
{

}