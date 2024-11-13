#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"BaseScene.h"
#include"SceneManager.h"
#include"GameObjectManager.h"
#include"UIBase.h"
#include "Transition.h"

//トランジションの初期化
Transition::Transition():
	m_alpha(0),
	m_isStartTransition(false),
	m_isFinishFadeout(false),
	m_isFinishFadein(false),
	m_scale(float(Json::GetInstance()->GetData()["TransitionScale"])),
	m_layerDepth(float(Json::GetInstance()->GetData()["TransitionLayerDepth"])),
	m_alphaAdd(float(Json::GetInstance()->GetData()["TransitionAlphaAdd"]))
{
	auto json = Json::GetInstance();
	auto deviceAccessor = DeviceAccessor::GetInstance();

	//テクスチャのロード
	SetCurrentDirectory(L"Assets/UI");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["TransitionTexture"]).c_str(),
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
Transition::~Transition()
{
	m_texture.Reset();
	m_textureResource.Reset();
}

//タイトルシーンオブジェクトの初期化(処理なし)
void Transition::InitializeTitle()
{

}

//タイトルシーンオブジェクトの更新
void Transition::UpdateTitle()
{
	Update();
}

//タイトルシーンオブジェクトの描画
void Transition::DrawTitle()
{
	Draw();
}

//UI初期化(処理なし)
void Transition::Initialize()
{

}

//UI更新(処理なし)
void Transition::Update()
{
	//シーン遷移が有効な場合はフェードアウト処理を行う
	if (m_isStartTransition)
	{
		if (!m_isFinishFadeout)
		{
			m_alpha += m_alphaAdd;
			m_isFinishFadein = false;
		}
		if (m_alpha >= 1.f)
		{
			m_isFinishFadeout = true;
		}
	}
	//シーン遷移が終わったらシーンをフェードインさせる
	else
	{
		m_isFinishFadeout = false;
		m_alpha -= m_alphaAdd;
		if (m_alpha <= 0.f)
		{
			m_alpha = 0.f;
			m_isFinishFadein = true;
		}
	}
}

//UI描画
void Transition::Draw()
{
	auto batch = GameObjectManager::GetInstance()->GetSpriteBatch();
	auto size = DeviceAccessor::GetInstance()->GetScreenSize();
	auto pos = Vector2(size.right / 2, size.bottom / 2);
	batch->Draw(m_texture.Get(), pos, nullptr, Colors::White * m_alpha, 0, m_origin, m_scale, SpriteEffects_None, m_layerDepth);
}

//リザルトシーンオブジェクトの初期化(処理なし)
void Transition::InitializeResult()
{

}

//リザルトシーンオブジェクトの更新
void Transition::UpdateResult()
{
	Update();
}

//リザルトシーンオブジェクトの描画
void Transition::DrawResult()
{
	Draw();
}