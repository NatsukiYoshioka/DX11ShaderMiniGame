#include "pch.h"
#include"GameObject.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"Block.h"
#include"BlockAccessor.h"
#include"Desk.h"
#include"DeskAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"Enemy.h"
#include"EnemyAccessor.h"
#include"DeviceAccessor.h"
#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::m_instance = nullptr;

//管理するオブジェクトの設定
GameObjectManager::GameObjectManager()
{
	EnemyAccessor::CreateInstance();
	CameraAccessor::CreateInstance();
	BlockAccessor::CreateInstance();
	DeskAccessor::CreateInstance();
	PlayerAccessor::CreateInstance();
	

	m_gameObjects.push_back(dynamic_cast<GameObject*>(EnemyAccessor::GetInstance()->GetEnemy()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(CameraAccessor::GetInstance()->GetCamera()));
	for (int i = 0; i < BlockAccessor::GetInstance()->GetBlocks().size(); i++)
	{
		m_gameObjects.push_back(dynamic_cast<GameObject*>(BlockAccessor::GetInstance()->GetBlocks().at(i)));
	}
	m_gameObjects.push_back(dynamic_cast<GameObject*>(DeskAccessor::GetInstance()->GetDesk()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(PlayerAccessor::GetInstance()->GetPlayer()));
	

	auto device = DeviceAccessor::GetInstance()->GetDevice();

	//オブジェクトシャドウ描画用デバイスの作成
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
}

//データ破棄
GameObjectManager::~GameObjectManager()
{
	if (m_gameObjects.empty())return;
	m_gameObjects.clear();

	CameraAccessor::DestroyInstance();
	BlockAccessor::DestroyInstance();
	DeskAccessor::DestroyInstance();
	PlayerAccessor::DestroyInstance();
	EnemyAccessor::DestroyInstance();
}

//インスタンス生成
void GameObjectManager::CreateInstance()
{
	if (m_instance)return;
	m_instance = new GameObjectManager();
}

//インスタンス破棄
void GameObjectManager::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}

//オブジェクトの更新
void GameObjectManager::Update()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->Update();
	}
}

//オブジェクトの描画
void GameObjectManager::Draw()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->Draw();
	}
}

void GameObjectManager::ClearObjectShadow()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->ClearDepthStencilView(m_objectShadowDepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void GameObjectManager::ClearCharacterShadow()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->ClearDepthStencilView(m_characterShadowDepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

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

void GameObjectManager::SetObjectShadowResource()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->PSSetShaderResources(10, 1, m_objectShadowView.GetAddressOf());
}

void GameObjectManager::SetCharacterShadowResource()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->PSSetShaderResources(11, 1, m_characterShadowView.GetAddressOf());
}

void GameObjectManager::DrawObjectShadow()
{
	for (int i = 0; i < BlockAccessor::GetInstance()->GetBlocks().size(); i++)
	{
		BlockAccessor::GetInstance()->GetBlocks().at(i)->DrawShadow();
	}
}

void GameObjectManager::DrawCharacterShadow()
{
	PlayerAccessor::GetInstance()->GetPlayer()->DrawShadow();
}