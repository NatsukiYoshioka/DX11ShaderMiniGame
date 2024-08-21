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
	BlockAccessor::CreateInstance();
	DeskAccessor::CreateInstance();
	PlayerAccessor::CreateInstance();
	EnemyAccessor::CreateInstance();
	CameraAccessor::CreateInstance();

	for (int i = 0; i < BlockAccessor::GetInstance()->GetBlocks().size(); i++)
	{
		m_gameObjects.push_back(dynamic_cast<GameObject*>(BlockAccessor::GetInstance()->GetBlocks().at(i)));
	}
	m_gameObjects.push_back(dynamic_cast<GameObject*>(DeskAccessor::GetInstance()->GetDesk()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(PlayerAccessor::GetInstance()->GetPlayer()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(EnemyAccessor::GetInstance()->GetEnemy()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(CameraAccessor::GetInstance()->GetCamera()));

	auto device = DeviceAccessor::GetInstance()->GetDevice();
	int width = DeviceAccessor::GetInstance()->GetScreenSize().right;
	int height = DeviceAccessor::GetInstance()->GetScreenSize().bottom;

	//オブジェクトシャドウ描画用デバイスの作成
	CD3D11_TEXTURE2D_DESC d(
		DXGI_FORMAT_R32_TYPELESS,
		width,
		height,
		1,
		1,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL,
		D3D11_USAGE_DEFAULT,
		0,
		1,
		0,
		0
	);
	device->CreateTexture2D(&d, NULL, m_shadowDepth.ReleaseAndGetAddressOf());

	CD3D11_DEPTH_STENCIL_VIEW_DESC ds(
		D3D11_DSV_DIMENSION_TEXTURE2D,
		DXGI_FORMAT_D32_FLOAT,
		0,
		0,
		1,
		0
	);
	device->CreateDepthStencilView(m_shadowDepth.Get(), &ds, m_shadowDepthView.ReleaseAndGetAddressOf());

	CD3D11_SHADER_RESOURCE_VIEW_DESC sd(
		D3D11_SRV_DIMENSION_TEXTURE2D,
		DXGI_FORMAT_R32_FLOAT,
		0,
		1,
		0,
		1
	);
	device->CreateShaderResourceView(m_shadowDepth.Get(), &sd, m_shadowView.ReleaseAndGetAddressOf());
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

void GameObjectManager::ClearShadow()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->ClearDepthStencilView(m_shadowDepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void GameObjectManager::SetShadowRenderTarget()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	ID3D11RenderTargetView* rtv = NULL;
	context->OMSetRenderTargets(1, &rtv, m_shadowDepthView.Get());

	int width = DeviceAccessor::GetInstance()->GetScreenSize().right;
	int height = DeviceAccessor::GetInstance()->GetScreenSize().bottom;
	CD3D11_VIEWPORT view(0.f, 0.f, float(width), float(height));
	context->RSSetViewports(1, &view);
}

void GameObjectManager::SetShadowResource()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->PSSetShaderResources(10, 1, m_shadowView.GetAddressOf());
}

void GameObjectManager::DrawShadow()
{
	for (int i = 0; i < BlockAccessor::GetInstance()->GetBlocks().size(); i++)
	{
		BlockAccessor::GetInstance()->GetBlocks().at(i)->DrawShadow();
	}
	PlayerAccessor::GetInstance()->GetPlayer()->DrawShadow();
}