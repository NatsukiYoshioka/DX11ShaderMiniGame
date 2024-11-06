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

//�Q�[���I�[�o�[��UI�G�t�F�N�g�̏�����
GameOverEffect::GameOverEffect():
	m_pos(),
	m_alpha(1.f),
	m_scale(float(Json::GetInstance()->GetData()["GameOverEffectScale"])),
	m_layerDepth(float(Json::GetInstance()->GetData()["GameOverEffectLayerDepth"]))
{
	auto deviceAccessor = DeviceAccessor::GetInstance();
	auto json = Json::GetInstance();

	//�e�N�X�`���̃��[�h
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

//�f�[�^�j��
GameOverEffect::~GameOverEffect()
{

}

//�^�C�g���V�[���I�u�W�F�N�g�̏�����(�����Ȃ�)
void GameOverEffect::InitializeTitle()
{

}

//�^�C�g���V�[���I�u�W�F�N�g�̍X�V(�����Ȃ�)
void GameOverEffect::UpdateTitle()
{

}

//�^�C�g���V�[���I�u�W�F�N�g�̕`��(�����Ȃ�)
void GameOverEffect::DrawTitle()
{

}

//UI������(�����Ȃ�)
void GameOverEffect::Initialize()
{

}

//UI�X�V(�����Ȃ�)
void GameOverEffect::Update()
{

}

//UI�`��(�����Ȃ�)
void GameOverEffect::Draw()
{

}

//���U���g�V�[���I�u�W�F�N�g�̏�����(�����Ȃ�)
void GameOverEffect::InitializeResult()
{

}

//���U���g�V�[���I�u�W�F�N�g�̍X�V
void GameOverEffect::UpdateResult()
{
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())return;
	//�l�`�̓��̈ړ������ɍ��킹�ăt�F�[�h�C��
	m_alpha = DollHeadAccessor::GetInstance()->GetDollHead()->GetPosRatio();
}

//���U���g�V�[���I�u�W�F�N�g�̕`��
void GameOverEffect::DrawResult()
{
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())return;
	auto batch = GameObjectManager::GetInstance()->GetSpriteBatch();

	batch->Draw(m_texture.Get(), m_pos, nullptr, Colors::White * m_alpha, 0, m_origin, m_scale, SpriteEffects_None, m_layerDepth);
}