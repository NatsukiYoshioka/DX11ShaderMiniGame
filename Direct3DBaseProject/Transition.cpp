#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"BaseScene.h"
#include"SceneManager.h"
#include"GameObjectManager.h"
#include"UIBase.h"
#include "Transition.h"

Transition::Transition():
	m_alpha(0),
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

	m_effect = make_unique<BasicEffect>(deviceAccessor->GetDevice());
	m_effect->SetTextureEnabled(true);
	m_effect->SetTexture(m_texture.Get());

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	DeviceAccessor::GetInstance()->GetDevice()->CreateInputLayout(
		VertexPositionColorTexture::InputElements,
		VertexPositionColorTexture::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());
}

Transition::~Transition()
{
	m_texture.Reset();
	m_textureResource.Reset();
	m_effect.reset();
}

//タイトルシーンオブジェクトの初期化
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

//UI初期化
void Transition::Initialize()
{

}

//UI更新
void Transition::Update()
{
	auto isChangeScene = SceneManager::GetInstance()->GetNowScene()->GetIsChangeScene();
	if (isChangeScene)
	{
		if (!m_isFinishFadeout)
		{
			m_alpha += m_alphaAdd * 2.f;
		}
		if (m_alpha >= 1.f)
		{
			m_isFinishFadeout = true;
		}
	}
	else
	{
		m_isFinishFadeout = false;
	}
	m_alpha -= m_alphaAdd;
	if (m_alpha <= 0.f)
	{
		m_alpha = 0.f;
		m_isFinishFadein = true;
	}
}

//UI描画
void Transition::Draw()
{
	auto batch = GameObjectManager::GetInstance()->GetSpriteBatch();
	auto size = DeviceAccessor::GetInstance()->GetScreenSize();
	auto pos = Vector2(0, 0);
	batch->End();
	batch->Begin(
		SpriteSortMode_Deferred, 
		DeviceAccessor::GetInstance()->GetStates()->NonPremultiplied(), 
		nullptr, DeviceAccessor::GetInstance()->GetStates()->DepthRead(), 
		DeviceAccessor::GetInstance()->GetStates()->CullNone(), [=]
		{
			m_effect->SetAlpha(m_alpha);
			m_effect->SetView(Matrix::Identity);
			m_effect->Apply(DeviceAccessor::GetInstance()->GetContext());
			DeviceAccessor::GetInstance()->GetContext()->IASetInputLayout(m_inputLayout.Get());
		});
	batch->Draw(m_texture.Get(), pos, nullptr, Colors::White, 0, m_origin, m_scale, SpriteEffects_None, m_layerDepth);
	batch->End();
	batch->Begin(SpriteSortMode_BackToFront);
}

//リザルトシーンオブジェクトの初期化
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