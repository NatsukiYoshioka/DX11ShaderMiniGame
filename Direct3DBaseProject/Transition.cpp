#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"BaseScene.h"
#include"SceneManager.h"
#include"GameObjectManager.h"
#include"UIBase.h"
#include "Transition.h"

//�g�����W�V�����̏�����
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

	//�e�N�X�`���̃��[�h
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

//�f�[�^�j��
Transition::~Transition()
{
	m_texture.Reset();
	m_textureResource.Reset();
}

//�^�C�g���V�[���I�u�W�F�N�g�̏�����(�����Ȃ�)
void Transition::InitializeTitle()
{

}

//�^�C�g���V�[���I�u�W�F�N�g�̍X�V
void Transition::UpdateTitle()
{
	Update();
}

//�^�C�g���V�[���I�u�W�F�N�g�̕`��
void Transition::DrawTitle()
{
	Draw();
}

//UI������(�����Ȃ�)
void Transition::Initialize()
{

}

//UI�X�V(�����Ȃ�)
void Transition::Update()
{
	//�V�[���J�ڂ��L���ȏꍇ�̓t�F�[�h�A�E�g�������s��
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
	//�V�[���J�ڂ��I�������V�[�����t�F�[�h�C��������
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

//UI�`��
void Transition::Draw()
{
	auto batch = GameObjectManager::GetInstance()->GetSpriteBatch();
	auto size = DeviceAccessor::GetInstance()->GetScreenSize();
	auto pos = Vector2(size.right / 2, size.bottom / 2);
	batch->Draw(m_texture.Get(), pos, nullptr, Colors::White * m_alpha, 0, m_origin, m_scale, SpriteEffects_None, m_layerDepth);
}

//���U���g�V�[���I�u�W�F�N�g�̏�����(�����Ȃ�)
void Transition::InitializeResult()
{

}

//���U���g�V�[���I�u�W�F�N�g�̍X�V
void Transition::UpdateResult()
{
	Update();
}

//���U���g�V�[���I�u�W�F�N�g�̕`��
void Transition::DrawResult()
{
	Draw();
}