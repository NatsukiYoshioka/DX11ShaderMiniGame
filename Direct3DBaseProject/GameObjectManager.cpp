#include "pch.h"
#include"Json.h"
#include"GameObject.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"Block.h"
#include"BlockAccessor.h"
#include"Desk.h"
#include"DeskAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"DollHead.h"
#include"DollHeadAccessor.h"
#include"Enemy.h"
#include"EnemyAccessor.h"
#include"GiftBox.h"
#include"GiftBoxAccessor.h"
#include"Room.h"
#include"RoomAccessor.h"
#include"UIBase.h"
#include"UIAccessor.h"
#include"DeviceAccessor.h"
#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::m_instance = nullptr;

//�Ǘ�����I�u�W�F�N�g�̐ݒ�
GameObjectManager::GameObjectManager()
{
	m_batch = make_unique<SpriteBatch>(DeviceAccessor::GetInstance()->GetContext());

	EnemyAccessor::CreateInstance();
	CameraAccessor::CreateInstance();
	BlockAccessor::CreateInstance();
	GiftBoxAccessor::CreateInstance();
	DeskAccessor::CreateInstance();
	PlayerAccessor::CreateInstance();
	DollHeadAccessor::CreateInstance();
	RoomAccessor::CreateInstance();
	UIAccessor::CreateInstance();

	m_gameObjects.push_back(dynamic_cast<GameObject*>(EnemyAccessor::GetInstance()->GetEnemy()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(GiftBoxAccessor::GetInstance()->GetGiftBox()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(CameraAccessor::GetInstance()->GetCamera()));
	for (int i = 0; i < BlockAccessor::GetInstance()->GetBlocks().size(); i++)
	{
		m_gameObjects.push_back(dynamic_cast<GameObject*>(BlockAccessor::GetInstance()->GetBlocks().at(i)));
	}
	m_gameObjects.push_back(dynamic_cast<GameObject*>(DeskAccessor::GetInstance()->GetDesk()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(PlayerAccessor::GetInstance()->GetPlayer()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(DollHeadAccessor::GetInstance()->GetDollHead()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(RoomAccessor::GetInstance()->GetRoom()));
	for (int i = 0; i < UIAccessor::GetInstance()->GetUIs().size(); i++)
	{
		m_gameObjects.push_back(dynamic_cast<GameObject*>(UIAccessor::GetInstance()->GetUIs().at(i)));
	}
	

	auto device = DeviceAccessor::GetInstance()->GetDevice();

	//�I�u�W�F�N�g�V���h�E�`��p�f�o�C�X�̍쐬
	CD3D11_TEXTURE2D_DESC d(
		DXGI_FORMAT_R32_TYPELESS,
		15000,
		15000,
		1,
		1,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL,
		D3D11_USAGE_DEFAULT,
		0,
		1,
		0,
		0
	);
	device->CreateTexture2D(&d, NULL, m_objectShadowDepth.ReleaseAndGetAddressOf());
	device->CreateTexture2D(&d, NULL, m_characterShadowDepth.ReleaseAndGetAddressOf());

	CD3D11_DEPTH_STENCIL_VIEW_DESC ds(
		D3D11_DSV_DIMENSION_TEXTURE2D,
		DXGI_FORMAT_D32_FLOAT,
		0,
		0,
		1,
		0
	);
	device->CreateDepthStencilView(m_objectShadowDepth.Get(), &ds, m_objectShadowDepthView.ReleaseAndGetAddressOf());
	device->CreateDepthStencilView(m_characterShadowDepth.Get(), &ds, m_characterShadowDepthView.ReleaseAndGetAddressOf());

	CD3D11_SHADER_RESOURCE_VIEW_DESC sd(
		D3D11_SRV_DIMENSION_TEXTURE2D,
		DXGI_FORMAT_R32_FLOAT,
		0,
		1,
		0,
		1
	);
	device->CreateShaderResourceView(m_objectShadowDepth.Get(), &sd, m_objectShadowView.ReleaseAndGetAddressOf());
	device->CreateShaderResourceView(m_characterShadowDepth.Get(), &sd, m_characterShadowView.ReleaseAndGetAddressOf());

	//�q�b�g����p�e�N�X�`���̍쐬
	auto size = int(Json::GetInstance()->GetData()["HitCheckTextureSize"]);
	CD3D11_TEXTURE2D_DESC h(
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		size,
		size,
		1,
		1,
		D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_RENDER_TARGET,
		D3D11_USAGE_DEFAULT,
		0,
		1,
		0,
		0
	);
	ComPtr<ID3D11Texture2D> hitCheckTex;
	ComPtr<ID3D11Texture2D> hitChectCharacterTex;
	device->CreateTexture2D(&h, NULL, hitCheckTex.ReleaseAndGetAddressOf());
	device->CreateTexture2D(&h, NULL, hitChectCharacterTex.ReleaseAndGetAddressOf());

	D3D11_RENDER_TARGET_VIEW_DESC rh = {};
	rh.Format = h.Format;
	rh.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	device->CreateRenderTargetView(hitCheckTex.Get(), &rh, m_hitCheckRenderTargetView.ReleaseAndGetAddressOf());
	device->CreateRenderTargetView(hitChectCharacterTex.Get(), &rh, m_hitCheckCharacterRTV.ReleaseAndGetAddressOf());

	CD3D11_SHADER_RESOURCE_VIEW_DESC sh(
		D3D11_SRV_DIMENSION_TEXTURE2D,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,
		1,
		0,
		1
	);
	ComPtr<ID3D11ShaderResourceView> hitCheckView;
	device->CreateShaderResourceView(hitCheckTex.Get(), &sh, m_hitCheckShaderResourceView.ReleaseAndGetAddressOf());
	device->CreateShaderResourceView(hitChectCharacterTex.Get(), &sh, m_hitCheckCharacterSRV.ReleaseAndGetAddressOf());

	CD3D11_TEXTURE2D_DESC dstDesc = {};
	dstDesc.Width = size;
	dstDesc.Height = size;
	dstDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	dstDesc.ArraySize = 1;
	dstDesc.MiscFlags = 0;
	dstDesc.Usage = D3D11_USAGE_DEFAULT;
	dstDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	dstDesc.MipLevels = 1;
	dstDesc.SampleDesc.Count = 1;
	dstDesc.SampleDesc.Quality = 0;
	ComPtr<ID3D11Texture2D> dst;
	device->CreateTexture2D(&dstDesc, NULL, dst.ReleaseAndGetAddressOf());

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	device->CreateDepthStencilView(dst.Get(), &dsvDesc, m_hitCheckDSV.ReleaseAndGetAddressOf());
}

//�f�[�^�j��
GameObjectManager::~GameObjectManager()
{
	if (m_gameObjects.empty())return;
	m_gameObjects.clear();

	CameraAccessor::DestroyInstance();
	BlockAccessor::DestroyInstance();
	GiftBoxAccessor::DestroyInstance();
	DeskAccessor::DestroyInstance();
	PlayerAccessor::DestroyInstance();
	DollHeadAccessor::DestroyInstance();
	EnemyAccessor::DestroyInstance();
	RoomAccessor::DestroyInstance();
}

//�C���X�^���X����
void GameObjectManager::CreateInstance()
{
	if (m_instance)return;
	m_instance = new GameObjectManager();
}

//�C���X�^���X�j��
void GameObjectManager::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}

//�^�C�g���V�[���I�u�W�F�N�g�̏�����
void GameObjectManager::InitializeTitle()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->InitializeTitle();
	}
}

//�^�C�g���V�[���I�u�W�F�N�g�̍X�V
void GameObjectManager::UpdateTitle()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->UpdateTitle();
	}
}

//�^�C�g���V�[���I�u�W�F�N�g�̕`��
void GameObjectManager::DrawTitle()
{
	m_batch->Begin(SpriteSortMode_BackToFront);
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->DrawTitle();
	}
	m_batch->End();
}

//�I�u�W�F�N�g�̏�����
void GameObjectManager::Initialize()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->Initialize();
	}
}

//�I�u�W�F�N�g�̍X�V
void GameObjectManager::Update()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->Update();
	}
}

//�I�u�W�F�N�g�̕`��
void GameObjectManager::Draw()
{
	m_batch->Begin(SpriteSortMode_BackToFront);
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->Draw();
	}
	m_batch->End();
}

//���U���g�V�[���I�u�W�F�N�g�̏�����
void GameObjectManager::InitializeResult()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->InitializeResult();
	}
}

//���U���g�V�[���I�u�W�F�N�g�̍X�V
void GameObjectManager::UpdateResult()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->UpdateResult();
	}
}

//���U���g�V�[���I�u�W�F�N�g�̕`��
void GameObjectManager::DrawResult()
{
	m_batch->Begin(SpriteSortMode_BackToFront);
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->DrawResult();
	}
	m_batch->End();
}

//�I�u�W�F�N�g�̉e�̏�����
void GameObjectManager::ClearObjectShadow()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->ClearDepthStencilView(m_objectShadowDepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

//�v���C���[�̉e�̏�����
void GameObjectManager::ClearCharacterShadow()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->ClearDepthStencilView(m_characterShadowDepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

//�I�u�W�F�N�g�̉e�̃����_�[�^�[�Q�b�g�ݒ�
void GameObjectManager::SetObjectShadowRenderTarget()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	ID3D11RenderTargetView* rtv = NULL;
	context->OMSetRenderTargets(1, &rtv, m_objectShadowDepthView.Get());

	int width = DeviceAccessor::GetInstance()->GetScreenSize().right;
	int height = DeviceAccessor::GetInstance()->GetScreenSize().bottom;
	CD3D11_VIEWPORT view(0.f, 0.f, float(15000), float(15000));
	context->RSSetViewports(1, &view);
}

//�v���C���[�̉e�̃����_�[�^�[�Q�b�g�ݒ�
void GameObjectManager::SetCharacterShadowRenderTarget()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	ID3D11RenderTargetView* rtv = NULL;
	context->OMSetRenderTargets(1, &rtv, m_characterShadowDepthView.Get());

	int width = DeviceAccessor::GetInstance()->GetScreenSize().right;
	int height = DeviceAccessor::GetInstance()->GetScreenSize().bottom;
	CD3D11_VIEWPORT view(0.f, 0.f, float(15000), float(15000));
	context->RSSetViewports(1, &view);
}

//�I�u�W�F�N�gSRV�ݒ�
void GameObjectManager::SetObjectShadowResource()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->PSSetShaderResources(10, 1, m_objectShadowView.GetAddressOf());
}

//�v���C���[SRV�ݒ�
void GameObjectManager::SetCharacterShadowResource()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->PSSetShaderResources(11, 1, m_characterShadowView.GetAddressOf());
}

//�I�u�W�F�N�g�̉e�`��
void GameObjectManager::DrawObjectShadow()
{
	RoomAccessor::GetInstance()->GetRoom()->DrawShadow();
	for (int i = 0; i < BlockAccessor::GetInstance()->GetBlocks().size(); i++)
	{
		BlockAccessor::GetInstance()->GetBlocks().at(i)->DrawShadow();
	}
	DeskAccessor::GetInstance()->GetDesk()->DrawShadow();
}

//�v���C���[�e�`��
void GameObjectManager::DrawCharacterShadow()
{
	PlayerAccessor::GetInstance()->GetPlayer()->DrawShadow();
}

//�G���_�̌����蔻��p�����_�����O�^�[�Q�b�g������
void GameObjectManager::ClearHitCheckRenderTarget()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->ClearRenderTargetView(m_hitCheckRenderTargetView.Get(), Colors::Black);
	context->ClearDepthStencilView(m_hitCheckDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

//�G���_�̌����蔻��p�v���C���[�����_�����O�^�[�Q�b�g������
void GameObjectManager::ClearHitCheckCharacterRenderTarget()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->ClearRenderTargetView(m_hitCheckCharacterRTV.Get(), Colors::Black);
	context->ClearDepthStencilView(m_hitCheckDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

//�G���_�̌����蔻��p�����_�����O�^�[�Q�b�g�ݒ�
void GameObjectManager::SetHitCheckRenderTarget()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();
	auto depthStencil = DeviceAccessor::GetInstance()->GetDepthStencilView();

	context->OMSetRenderTargets(1, m_hitCheckRenderTargetView.GetAddressOf(), m_hitCheckDSV.Get());

	auto size = int(Json::GetInstance()->GetData()["HitCheckTextureSize"]);
	CD3D11_VIEWPORT view(0.f, 0.f, float(size), float(size));
	context->RSSetViewports(1, &view);
}

//�G���_�̌����蔻��p�v���C���[�����_�����O�^�[�Q�b�g�ݒ�
void GameObjectManager::SetHitCheckCharacterRenderTarget()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();
	auto depthStencil = DeviceAccessor::GetInstance()->GetDepthStencilView();

	context->OMSetRenderTargets(1, m_hitCheckCharacterRTV.GetAddressOf(), m_hitCheckDSV.Get());

	auto size = int(Json::GetInstance()->GetData()["HitCheckTextureSize"]);
	CD3D11_VIEWPORT view(0.f, 0.f, float(size), float(size));
	context->RSSetViewports(1, &view);
}

//�����蔻��p�e�N�X�`��SRV�ݒ�
void GameObjectManager::SetHitCheckShaderResource()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->CSSetShaderResources(1, 1, m_hitCheckShaderResourceView.GetAddressOf());
}

//�����蔻��p�v���C���[�e�N�X�`��SRV�ݒ�
void GameObjectManager::SetHitCheckCharacterShaderResource()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->CSSetShaderResources(0, 1, m_hitCheckCharacterSRV.GetAddressOf());
}

//�����蔻��p�I�u�W�F�N�g�`��
void GameObjectManager::DrawHitCheck()
{
	for (int i = 0; i < BlockAccessor::GetInstance()->GetBlocks().size(); i++)
	{
		auto playerPos = PlayerAccessor::GetInstance()->GetPlayer()->GetPos();
		auto blockPos = BlockAccessor::GetInstance()->GetBlocks().at(i)->GetPos();
		if (Vector2::Distance(Vector2(blockPos.x, blockPos.z), Vector2(playerPos.x, playerPos.z)) > 4.f)continue;
		BlockAccessor::GetInstance()->GetBlocks().at(i)->DrawHitCheck();
	}
	PlayerAccessor::GetInstance()->GetPlayer()->DrawHitCheck();
}

//�����蔻��p�v���C���[�`��
void GameObjectManager::DrawHitCheckCharacter()
{
	PlayerAccessor::GetInstance()->GetPlayer()->DrawHitCheck();
}

//�����蔻�菈��
void GameObjectManager::HitCheck()
{
	PlayerAccessor::GetInstance()->GetPlayer()->HitCheck();
}