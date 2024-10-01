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

	//�e�N�X�`���̃��[�h
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

//�^�C�g���V�[���I�u�W�F�N�g�̏�����
void ControlUI::InitializeTitle()
{

}

//�^�C�g���V�[���I�u�W�F�N�g�̍X�V
void ControlUI::UpdateTitle()
{

}

//�^�C�g���V�[���I�u�W�F�N�g�̕`��
void ControlUI::DrawTitle()
{

}

//UI������
void ControlUI::Initialize()
{

}

//UI�X�V
void ControlUI::Update()
{

}

//UI�`��
void ControlUI::Draw()
{
	auto batch = GameObjectManager::GetInstance()->GetSpriteBatch();

	batch->Draw(m_texture.Get(), m_pos, nullptr, Colors::White, 0, m_origin, m_scale, SpriteEffects_None, m_layerDepth);
}

//���U���g�V�[���I�u�W�F�N�g�̏�����
void ControlUI::InitializeResult()
{

}

//���U���g�V�[���I�u�W�F�N�g�̍X�V
void ControlUI::UpdateResult()
{

}

//���U���g�V�[���I�u�W�F�N�g�̕`��
void ControlUI::DrawResult()
{
	
}