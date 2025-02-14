#include "pch.h"
#include"Json.h"
#include"GameObject.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"Block.h"
#include"BlockAccessor.h"
#include"GoalObject.h"
#include"GoalObjectAccessor.h"
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
#include"SoundBase.h"
#include"SoundAccessor.h"
#include"DeviceAccessor.h"
#include<random>
#include"ReadData.h"
#include"FoundUI.h"
#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::m_instance = nullptr;

float GameObjectManager::RandomFloat(float min, float max)
{
	default_random_engine engine(static_cast<unsigned int>(time(0)));
	uniform_real_distribution<float> dist(min, max);
	return dist(engine);
}

float GameObjectManager::Lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

//管理するオブジェクトの設定
GameObjectManager::GameObjectManager():
	m_AOConstantBuffer(DeviceAccessor::GetInstance()->GetDevice()),
	m_LUTConstantBuffer(DeviceAccessor::GetInstance()->GetDevice()),
	m_blurBuffer(DeviceAccessor::GetInstance()->GetDevice()),
	m_shadowTextureSize(float(Json::GetInstance()->GetData()["ShadowTextureSize"]))
{
	m_batch = make_unique<SpriteBatch>(DeviceAccessor::GetInstance()->GetContext());

	EnemyAccessor::CreateInstance();
	CameraAccessor::CreateInstance();
	BlockAccessor::CreateInstance();
	GiftBoxAccessor::CreateInstance();
	GoalObjectAccessor::CreateInstance();
	DeskAccessor::CreateInstance();
	PlayerAccessor::CreateInstance();
	DollHeadAccessor::CreateInstance();
	RoomAccessor::CreateInstance();
	UIAccessor::CreateInstance();
	SoundAccessor::CreateInstance();

	//オブジェクトの追加
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
	for (int i = 0;i < SoundAccessor::GetInstance()->GetSounds().size();i++)
	{
		m_gameObjects.push_back(dynamic_cast<GameObject*>(SoundAccessor::GetInstance()->GetSounds().at(i)));
	}

	CreateShadowDevice();

	CreateFindCheckDevice();

	CreateSpriteRenderDevice();

	CreateAmbientOcclusionDevice();

	CreateLUTDevice();

	CreateBloomDevice();
}

//データ破棄
GameObjectManager::~GameObjectManager()
{
	if (m_gameObjects.empty())return;
	m_gameObjects.clear();

	CameraAccessor::DestroyInstance();
	BlockAccessor::DestroyInstance();
	GiftBoxAccessor::DestroyInstance();
	GoalObjectAccessor::DestroyInstance();
	DeskAccessor::DestroyInstance();
	PlayerAccessor::DestroyInstance();
	DollHeadAccessor::DestroyInstance();
	EnemyAccessor::DestroyInstance();
	RoomAccessor::DestroyInstance();
	UIAccessor::DestroyInstance();
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

//タイトルシーンオブジェクトの初期化
void GameObjectManager::InitializeTitle()
{
	m_gameObjects.clear();
	m_gameObjects.push_back(dynamic_cast<GameObject*>(EnemyAccessor::GetInstance()->GetEnemy()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(GiftBoxAccessor::GetInstance()->GetGiftBox()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(CameraAccessor::GetInstance()->GetCamera()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(PlayerAccessor::GetInstance()->GetPlayer()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(RoomAccessor::GetInstance()->GetRoom()));
	for (int i = 0; i < UIAccessor::GetInstance()->GetUIs().size(); i++)
	{
		m_gameObjects.push_back(dynamic_cast<GameObject*>(UIAccessor::GetInstance()->GetUIs().at(i)));
	}
	for (int i = 0; i < SoundAccessor::GetInstance()->GetSounds().size(); i++)
	{
		m_gameObjects.push_back(dynamic_cast<GameObject*>(SoundAccessor::GetInstance()->GetSounds().at(i)));
	}
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->InitializeTitle();
	}
}

//タイトルシーンオブジェクトの更新
void GameObjectManager::UpdateTitle()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->UpdateTitle();
	}
}

//タイトルシーンオブジェクトの描画
void GameObjectManager::DrawTitle()
{
	auto states = DeviceAccessor::GetInstance()->GetStates();
	auto deviceAccessor = DeviceAccessor::GetInstance();
	float clearColor[4] = { 0.f,0.f,0.f,0.f };
	deviceAccessor->GetContext()->ClearRenderTargetView(m_spriteRTV.Get(), clearColor);
	m_batch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied(),
		nullptr, nullptr, nullptr, [=]
		{
			deviceAccessor->GetContext()->OMSetRenderTargets(1, m_spriteRTV.GetAddressOf(), nullptr);
			deviceAccessor->GetContext()->PSSetShader(m_spritePixel.Get(), NULL, 0);
		});
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->DrawTitle();
	}
	m_batch->End();
}

//オブジェクトの初期化
void GameObjectManager::Initialize()
{
	m_gameObjects.clear();
	m_gameObjects.push_back(dynamic_cast<GameObject*>(EnemyAccessor::GetInstance()->GetEnemy()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(CameraAccessor::GetInstance()->GetCamera()));
	for (int i = 0; i < BlockAccessor::GetInstance()->GetBlocks().size(); i++)
	{
		m_gameObjects.push_back(dynamic_cast<GameObject*>(BlockAccessor::GetInstance()->GetBlocks().at(i)));
	}
	m_gameObjects.push_back(dynamic_cast<GameObject*>(GoalObjectAccessor::GetInstance()->GetGoalObject()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(DeskAccessor::GetInstance()->GetDesk()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(PlayerAccessor::GetInstance()->GetPlayer()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(RoomAccessor::GetInstance()->GetRoom()));
	for (int i = 0; i < UIAccessor::GetInstance()->GetUIs().size(); i++)
	{
		m_gameObjects.push_back(dynamic_cast<GameObject*>(UIAccessor::GetInstance()->GetUIs().at(i)));
	}
	for (int i = 0; i < SoundAccessor::GetInstance()->GetSounds().size(); i++)
	{
		m_gameObjects.push_back(dynamic_cast<GameObject*>(SoundAccessor::GetInstance()->GetSounds().at(i)));
	}

	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->Initialize();
	}
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
	auto states = DeviceAccessor::GetInstance()->GetStates();
	auto deviceAccessor = DeviceAccessor::GetInstance();
	float clearColor[4] = { 0.f,0.f,0.f,0.f };
	deviceAccessor->GetContext()->ClearRenderTargetView(m_spriteRTV.Get(), clearColor);
	m_batch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied(),
		nullptr, nullptr, nullptr, [=]
		{
			deviceAccessor->GetContext()->OMSetRenderTargets(1, m_spriteRTV.GetAddressOf(), nullptr);
			deviceAccessor->GetContext()->PSSetShader(m_spritePixel.Get(), NULL, 0);
		});
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->Draw();
	}
	m_batch->End();
}

//リザルトシーンオブジェクトの初期化
void GameObjectManager::InitializeResult()
{
	m_gameObjects.clear();
	m_gameObjects.push_back(dynamic_cast<GameObject*>(EnemyAccessor::GetInstance()->GetEnemy()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(CameraAccessor::GetInstance()->GetCamera()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(DeskAccessor::GetInstance()->GetDesk()));
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())
	{
		m_gameObjects.push_back(dynamic_cast<GameObject*>(PlayerAccessor::GetInstance()->GetPlayer()));
	}
	m_gameObjects.push_back(dynamic_cast<GameObject*>(DollHeadAccessor::GetInstance()->GetDollHead()));
	m_gameObjects.push_back(dynamic_cast<GameObject*>(RoomAccessor::GetInstance()->GetRoom()));
	for (int i = 0; i < UIAccessor::GetInstance()->GetUIs().size(); i++)
	{
		m_gameObjects.push_back(dynamic_cast<GameObject*>(UIAccessor::GetInstance()->GetUIs().at(i)));
	}
	for (int i = 0; i < SoundAccessor::GetInstance()->GetSounds().size(); i++)
	{
		m_gameObjects.push_back(dynamic_cast<GameObject*>(SoundAccessor::GetInstance()->GetSounds().at(i)));
	}

	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->InitializeResult();
	}
}

//リザルトシーンオブジェクトの更新
void GameObjectManager::UpdateResult()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->UpdateResult();
	}
}

//リザルトシーンオブジェクトの描画
void GameObjectManager::DrawResult()
{
	auto states = DeviceAccessor::GetInstance()->GetStates();
	auto deviceAccessor = DeviceAccessor::GetInstance();
	float clearColor[4] = { 0.f,0.f,0.f,0.f };
	deviceAccessor->GetContext()->ClearRenderTargetView(m_spriteRTV.Get(), clearColor);
	m_batch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied(),
		nullptr, nullptr, nullptr, [=]
		{
			deviceAccessor->GetContext()->OMSetRenderTargets(1, m_spriteRTV.GetAddressOf(), nullptr);
			deviceAccessor->GetContext()->PSSetShader(m_spritePixel.Get(), NULL, 0);
		});
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->DrawResult();
	}
	m_batch->End();
}

void GameObjectManager::CreateShadowDevice()
{
	auto device = DeviceAccessor::GetInstance()->GetDevice();
	//オブジェクトシャドウ描画用デバイスの作成
	CD3D11_TEXTURE2D_DESC textureDesc(
		DXGI_FORMAT_R32_TYPELESS,
		m_shadowTextureSize,
		m_shadowTextureSize,
		1,
		1,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL,
		D3D11_USAGE_DEFAULT,
		0,
		1,
		0,
		0
	);
	device->CreateTexture2D(&textureDesc, NULL, m_objectShadowDepth.ReleaseAndGetAddressOf());

	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc(
		D3D11_DSV_DIMENSION_TEXTURE2D,
		DXGI_FORMAT_D32_FLOAT,
		0,
		0,
		1,
		0
	);
	device->CreateDepthStencilView(m_objectShadowDepth.Get(), &dsvDesc, m_objectShadowDepthView.ReleaseAndGetAddressOf());

	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(
		D3D11_SRV_DIMENSION_TEXTURE2D,
		DXGI_FORMAT_R32_FLOAT,
		0,
		1,
		0,
		1
	);
	device->CreateShaderResourceView(m_objectShadowDepth.Get(), &srvDesc, m_objectShadowView.ReleaseAndGetAddressOf());
}

//オブジェクトの影の初期化
void GameObjectManager::ClearObjectShadow()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->ClearDepthStencilView(m_objectShadowDepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

//オブジェクトの影のレンダーターゲット設定
void GameObjectManager::SetObjectShadowRenderTarget()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	ID3D11RenderTargetView* rtv = NULL;
	context->OMSetRenderTargets(1, &rtv, m_objectShadowDepthView.Get());

	CD3D11_VIEWPORT view(0.f, 0.f, m_shadowTextureSize, m_shadowTextureSize);
	context->RSSetViewports(1, &view);
}

//オブジェクトSRV設定
void GameObjectManager::SetObjectShadowResource()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->PSSetShaderResources(10, 1, m_objectShadowView.GetAddressOf());
}

//影の描画
void GameObjectManager::DrawShadow()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->DrawShadow();
	}
}

//スプライト用デバイス作成
void GameObjectManager::CreateSpriteRenderDevice()
{
	auto device = DeviceAccessor::GetInstance()->GetDevice();
	
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = DeviceAccessor::GetInstance()->GetScreenSize().right;
	textureDesc.Height = DeviceAccessor::GetInstance()->GetScreenSize().bottom;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	device->CreateTexture2D(&textureDesc, nullptr, m_spriteTexture.ReleaseAndGetAddressOf());

	device->CreateRenderTargetView(m_spriteTexture.Get(), nullptr, m_spriteRTV.ReleaseAndGetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(m_spriteTexture.Get(), &srvDesc, m_spriteSRV.ReleaseAndGetAddressOf());
}

//スプライトの最終描画
void GameObjectManager::DrawSprite()
{
	auto states = DeviceAccessor::GetInstance()->GetStates();
	m_batch->Begin(SpriteSortMode_Deferred, states->AlphaBlend());
	m_batch->Draw(m_spriteSRV.Get(), Vector2(0, 0));
	m_batch->End();
}

//SSAO描画用デバイスの作成
void GameObjectManager::CreateAmbientOcclusionDevice()
{
	auto device = DeviceAccessor::GetInstance()->GetDevice();
	//法線+深度値保存用ビューの作成
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = DeviceAccessor::GetInstance()->GetScreenSize().right;
	textureDesc.Height = DeviceAccessor::GetInstance()->GetScreenSize().bottom;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	device->CreateTexture2D(&textureDesc, nullptr, m_normalDepthTexture.ReleaseAndGetAddressOf());

	device->CreateRenderTargetView(m_normalDepthTexture.Get(), nullptr, m_normalDepthRTV.ReleaseAndGetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(m_normalDepthTexture.Get(), &srvDesc, m_normalDepthSRV.ReleaseAndGetAddressOf());

	//AO用シェーダーの作成
	SetCurrentDirectory(L"Assets/Shader");
	auto vsBlob = DX::ReadData(L"PostProccessVertex.cso");
	auto psBlob = DX::ReadData(L"AmbientOcclusionPixel.cso");

	device->CreateVertexShader(vsBlob.data(), vsBlob.size(), nullptr, m_PostProccessVertex.ReleaseAndGetAddressOf());
	device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_AOPixel.ReleaseAndGetAddressOf());

	psBlob = DX::ReadData(L"SpritePixel.cso");
	device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_spritePixel.ReleaseAndGetAddressOf());
	SetCurrentDirectory(L"../../");


	//AO用定数バッファの初期化
	AOConstants constants;
	//AO用サンプルカーネルの初期化
	vector<XMFLOAT2> poissonDisk = {
		XMFLOAT2(-0.94201624f, -0.39906216f), XMFLOAT2(0.94558609f, -0.76890725f),
		XMFLOAT2(-0.094184101f, -0.92938870f), XMFLOAT2(0.34495938f, 0.29387760f),
		XMFLOAT2(-0.91588581f, 0.45771432f), XMFLOAT2(-0.81544232f, -0.87912464f),
		XMFLOAT2(-0.38277543f, 0.27676845f), XMFLOAT2(0.97484398f, 0.75648379f),
		XMFLOAT2(0.44323325f, -0.97511554f), XMFLOAT2(0.53742981f, -0.47373420f),
		XMFLOAT2(-0.26496911f, 0.41893023f), XMFLOAT2(0.79197514f, 0.19090188f),
		XMFLOAT2(-0.24188840f, 0.99706507f), XMFLOAT2(-0.81409955f, 0.91437590f),
		XMFLOAT2(0.19984126f, 0.78641367f), XMFLOAT2(0.14383161f, -0.14100790f)
	};
	for (int i = 0; i < 64; i++)
	{
		Vector4 sample(
			poissonDisk[i % poissonDisk.size()].x, poissonDisk[i % poissonDisk.size()].y, RandomFloat(0.f, 1.f), 0
		);
		sample = XMVector4Normalize(XMLoadFloat4(&sample));
		sample = XMVectorScale(XMLoadFloat4(&sample), RandomFloat(0.0f, 1.0f));
		float scale = float(i) / 64.0f;
		scale = Lerp(0.1f, 1.0f, scale * scale);
		sample = XMVectorScale(XMLoadFloat4(&sample), scale);
		constants.sampleKernel[i] = sample;
	}
	constants.projection = CameraAccessor::GetInstance()->GetCamera()->GetProjection();
	constants.inverseProjection = XMMatrixInverse(nullptr, CameraAccessor::GetInstance()->GetCamera()->GetProjection());
	constants.radius = float(Json::GetInstance()->GetData()["SSAORadius"]);
	constants.ZFar = float(Json::GetInstance()->GetData()["SSAOZFar"]);
	constants.AOPower = float(Json::GetInstance()->GetData()["SSAOPower"]);
	m_AOConstantBuffer.SetData(DeviceAccessor::GetInstance()->GetContext(), constants);

	//ポストプロセス用スクリーンの頂点,インデックスデータ設定
	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f,  1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }, // 左上
		{ XMFLOAT3(1.0f,  1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) }, // 右上
		{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }, // 左下
		{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }
	};

	UINT indices[] =
	{
		0,1,2,2,1,3
	};

	//AO用頂点バッファの作成
	D3D11_BUFFER_DESC vbufferDesc = {};
	vbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vbufferDesc.ByteWidth = sizeof(vertices);
	vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;

	device->CreateBuffer(&vbufferDesc, &initData, m_vertexBuffer.ReleaseAndGetAddressOf());

	//AO用インデックスバッファ作成
	D3D11_BUFFER_DESC ibufferDesc = {};
	ibufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ibufferDesc.ByteWidth = sizeof(indices);
	ibufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices;
	device->CreateBuffer(&ibufferDesc, &indexData, m_indexBuffer.ReleaseAndGetAddressOf());

	//入力レイアウトの作成
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		 { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		 { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	device->CreateInputLayout(layout, ARRAYSIZE(layout),
		vsBlob.data(), vsBlob.size(), m_inputLayout.ReleaseAndGetAddressOf());

	//AOブレンドステートを作成
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&blendDesc, m_AOBlend.ReleaseAndGetAddressOf());
}

//SSAO描画(ポストプロセス)
void GameObjectManager::DrawAmbientOcclusion()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->IASetInputLayout(m_inputLayout.Get());
	context->VSSetShader(m_PostProccessVertex.Get(), NULL, 0);
	context->PSSetShader(m_AOPixel.Get(), NULL, 0);
	auto states = DeviceAccessor::GetInstance()->GetStates();
	ID3D11SamplerState* samplers[] =
	{
		states->PointClamp(),
		states->PointClamp(),
	};
	context->PSSetSamplers(0, 1, samplers);
	context->PSSetShaderResources(0, 1, m_normalDepthSRV.GetAddressOf());

	context->OMSetBlendState(m_AOBlend.Get(), 0, 0xffffffff);
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	auto cb = m_AOConstantBuffer.GetBuffer();
	context->PSSetConstantBuffers(4, 1, &cb);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->DrawIndexed(6, 0, 0);
}

//見つかり判定用デバイスの作成
void GameObjectManager::CreateFindCheckDevice()
{
	auto device = DeviceAccessor::GetInstance()->GetDevice();
	//ヒット判定用テクスチャの作成
	auto size = int(Json::GetInstance()->GetData()["HitCheckTextureSize"]);
	CD3D11_TEXTURE2D_DESC textureDesc(
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		size,
		size,
		1,
		1,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		D3D11_USAGE_DEFAULT,
		0,
		1,
		0,
		0
	);
	ComPtr<ID3D11Texture2D> hitCheckTex;
	ComPtr<ID3D11Texture2D> hitChectCharacterTex;
	device->CreateTexture2D(&textureDesc, NULL, hitCheckTex.ReleaseAndGetAddressOf());
	device->CreateTexture2D(&textureDesc, NULL, hitChectCharacterTex.ReleaseAndGetAddressOf());

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	device->CreateRenderTargetView(hitCheckTex.Get(), &rtvDesc, m_hitCheckRenderTargetView.ReleaseAndGetAddressOf());
	device->CreateRenderTargetView(hitChectCharacterTex.Get(), &rtvDesc, m_hitCheckCharacterRTV.ReleaseAndGetAddressOf());

	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(
		D3D11_SRV_DIMENSION_TEXTURE2D,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,
		1,
		0,
		1
	);
	ComPtr<ID3D11ShaderResourceView> hitCheckView;
	device->CreateShaderResourceView(hitCheckTex.Get(), &srvDesc, m_hitCheckShaderResourceView.ReleaseAndGetAddressOf());
	device->CreateShaderResourceView(hitChectCharacterTex.Get(), &srvDesc, m_hitCheckCharacterSRV.ReleaseAndGetAddressOf());

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

//敵視点の見つかり判定用レンダリングターゲット初期化
void GameObjectManager::ClearHitCheckRenderTarget()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->ClearRenderTargetView(m_hitCheckRenderTargetView.Get(), Colors::Black);
	context->ClearDepthStencilView(m_hitCheckDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

//敵視点の見つかり判定用プレイヤーレンダリングターゲット初期化
void GameObjectManager::ClearHitCheckCharacterRenderTarget()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->ClearRenderTargetView(m_hitCheckCharacterRTV.Get(), Colors::Black);
	context->ClearDepthStencilView(m_hitCheckDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

//敵視点の見つかり判定用レンダリングターゲット設定
void GameObjectManager::SetHitCheckRenderTarget()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->OMSetRenderTargets(1, m_hitCheckRenderTargetView.GetAddressOf(), m_hitCheckDSV.Get());

	auto size = int(Json::GetInstance()->GetData()["HitCheckTextureSize"]);
	CD3D11_VIEWPORT view(0.f, 0.f, float(size), float(size));
	context->RSSetViewports(1, &view);
}

//敵視点の見つかり判定用プレイヤーレンダリングターゲット設定
void GameObjectManager::SetHitCheckCharacterRenderTarget()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->OMSetRenderTargets(1, m_hitCheckCharacterRTV.GetAddressOf(), m_hitCheckDSV.Get());

	auto size = int(Json::GetInstance()->GetData()["HitCheckTextureSize"]);
	CD3D11_VIEWPORT view(0.f, 0.f, float(size), float(size));
	context->RSSetViewports(1, &view);
}

//見つかり判定用テクスチャSRV設定
void GameObjectManager::SetHitCheckShaderResource()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->CSSetShaderResources(1, 1, m_hitCheckShaderResourceView.GetAddressOf());
}

//見つかり判定用プレイヤーテクスチャSRV設定
void GameObjectManager::SetHitCheckCharacterShaderResource()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->CSSetShaderResources(0, 1, m_hitCheckCharacterSRV.GetAddressOf());
}

//見つかり判定用オブジェクト描画
void GameObjectManager::DrawHitCheck()
{
	for (int i = 0; i < BlockAccessor::GetInstance()->GetBlocks().size(); i++)
	{
		BlockAccessor::GetInstance()->GetBlocks().at(i)->DrawHitCheck();
	}
	PlayerAccessor::GetInstance()->GetPlayer()->DrawHitCheck();
}

//見つかり判定用プレイヤー描画
void GameObjectManager::DrawHitCheckCharacter()
{
	PlayerAccessor::GetInstance()->GetPlayer()->DrawHitCheck();
}

//見つかり判定処理
void GameObjectManager::HitCheck()
{
	PlayerAccessor::GetInstance()->GetPlayer()->HitCheck();
}

//LUT描画用デバイスの作成
void GameObjectManager::CreateLUTDevice()
{
	auto device = DeviceAccessor::GetInstance()->GetDevice();

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = DeviceAccessor::GetInstance()->GetScreenSize().right;
	textureDesc.Height = DeviceAccessor::GetInstance()->GetScreenSize().bottom;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	device->CreateTexture2D(&textureDesc, nullptr, m_LUTColorTexture.ReleaseAndGetAddressOf());

	device->CreateRenderTargetView(m_LUTColorTexture.Get(), nullptr, m_LUTColorRTV.ReleaseAndGetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(m_LUTColorTexture.Get(), &srvDesc, m_LUTColorSRV.ReleaseAndGetAddressOf());

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, m_LUTSampler.ReleaseAndGetAddressOf());

	auto json = Json::GetInstance();
	SetCurrentDirectory(L"Assets/Shader");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device,
		json->Widen(json->GetData()["LUTTexture"]).c_str(),
		nullptr,
		m_LUTSampleSRV.ReleaseAndGetAddressOf()));
	auto psBlob = DX::ReadData(L"LookUpTablePixel.cso");
	device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_LUTPixel.ReleaseAndGetAddressOf());
	SetCurrentDirectory(L"../../");
}

//LUT描画(ポストプロセス)
void GameObjectManager::DrawLUT()
{
	auto context = DeviceAccessor::GetInstance()->GetContext();

	context->IASetInputLayout(m_inputLayout.Get());
	context->VSSetShader(m_PostProccessVertex.Get(), NULL, 0);
	context->PSSetShader(m_LUTPixel.Get(), NULL, 0);
	auto states = DeviceAccessor::GetInstance()->GetStates();

	context->PSSetSamplers(0, 1, m_LUTSampler.GetAddressOf());
	context->PSSetShaderResources(0, 1, m_LUTColorSRV.GetAddressOf());
	context->PSSetShaderResources(1, 1, m_LUTSampleSRV.GetAddressOf());
	context->PSSetShaderResources(2, 1, m_spriteSRV.GetAddressOf());

	context->OMSetBlendState(states->Opaque(), 0, 0xffffffff);
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	LUTConstants constants = {};
	for (int i = 0; i < UIAccessor::GetInstance()->GetUIs().size(); i++)
	{
		auto foundUI = dynamic_cast<FoundUI*>(UIAccessor::GetInstance()->GetUIs().at(i));
		if (foundUI)
		{
			constants.gradingPower = foundUI->GetTimeRatio() * float(Json::GetInstance()->GetData()["LUTMaxPower"]);
			break;
		}
	}
	m_LUTConstantBuffer.SetData(context, constants);
	auto cb = m_LUTConstantBuffer.GetBuffer();
	context->PSSetConstantBuffers(4, 1, &cb);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->DrawIndexed(6, 0, 0);
}

//ブルーム用デバイス作成
void GameObjectManager::CreateBloomDevice()
{
	auto device = DeviceAccessor::GetInstance()->GetDevice();

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = DeviceAccessor::GetInstance()->GetScreenSize().right;
	textureDesc.Height = DeviceAccessor::GetInstance()->GetScreenSize().bottom;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	device->CreateTexture2D(&textureDesc, nullptr, m_bloomTexture.ReleaseAndGetAddressOf());

	device->CreateRenderTargetView(m_bloomTexture.Get(), nullptr, m_bloomRTV.ReleaseAndGetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(m_bloomTexture.Get(), &srvDesc, m_bloomSRV.ReleaseAndGetAddressOf());

	//ガウシアンブラーの重み計算
	BlurConstants constants = {};
	float total = 0.f;

	//ガウス関数を用いて重み計算をする
	//iが基準テクセルからの距離
	for (int i = 0;i < 8;i++)
	{
		constants.weights[i] = expf(-0.5f * (float)(i * i) / 8.f);
		total += 2.f * constants.weights[i];
	}

	//重みの合計で除算して、合計を1にする
	for (int i = 0;i < 8;i++)
	{
		constants.weights[i] /= total;
	}
	m_blurBuffer.SetData(DeviceAccessor::GetInstance()->GetContext(), constants);

	//ブラー用デバイスの作成
	for (int i = 0;i < 4;i++)
	{
		float width = DeviceAccessor::GetInstance()->GetScreenSize().right;
		float height = DeviceAccessor::GetInstance()->GetScreenSize().bottom;
		for (int j = 0;j < i;j++)
		{
			width /= 2.f;
			height /= 2.f;
		}
		CreateBlurDevice(
			width,
			height,
			m_blurTexture[i],
			m_blurRTV[i],
			m_blurSRV[i]);
	}
}

//ブラー処理デバイス作成
void GameObjectManager::CreateBlurDevice(float exWidth, float exHeight,
	ComPtr<ID3D11Texture2D> texture[],
	ComPtr<ID3D11RenderTargetView> rtv[],
	ComPtr<ID3D11ShaderResourceView> srv[])
{
	auto device = DeviceAccessor::GetInstance()->GetDevice();

	//横ブラー用デバイス作成
	D3D11_TEXTURE2D_DESC xBlurTextureDesc = {};
	xBlurTextureDesc.Width = exWidth / 2.f;
	xBlurTextureDesc.Height = exHeight;
	xBlurTextureDesc.MipLevels = 1;
	xBlurTextureDesc.ArraySize = 1;
	xBlurTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	xBlurTextureDesc.SampleDesc.Count = 1;
	xBlurTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	xBlurTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	//縦ブラー用デバイス作成
	D3D11_TEXTURE2D_DESC yBlurTextureDesc = {};
	yBlurTextureDesc.Width = exWidth / 2.f;
	yBlurTextureDesc.Height = exHeight / 2.f;
	yBlurTextureDesc.MipLevels = 1;
	yBlurTextureDesc.ArraySize = 1;
	yBlurTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	yBlurTextureDesc.SampleDesc.Count = 1;
	yBlurTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	yBlurTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	device->CreateTexture2D(&xBlurTextureDesc, nullptr, texture[0].ReleaseAndGetAddressOf());
	device->CreateTexture2D(&yBlurTextureDesc, nullptr, texture[1].ReleaseAndGetAddressOf());

	device->CreateRenderTargetView(texture[0].Get(), nullptr, rtv[0].ReleaseAndGetAddressOf());
	device->CreateRenderTargetView(texture[1].Get(), nullptr, rtv[1].ReleaseAndGetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(texture[0].Get(), &srvDesc, srv[0].ReleaseAndGetAddressOf());
	device->CreateShaderResourceView(texture[1].Get(), &srvDesc, srv[1].ReleaseAndGetAddressOf());

	SetCurrentDirectory(L"Assets/Shader");
	auto vsBlob = DX::ReadData(L"XBlurVertex.cso");
	device->CreateVertexShader(vsBlob.data(), vsBlob.size(), nullptr, m_xBlurVertex.ReleaseAndGetAddressOf());
	vsBlob = DX::ReadData(L"YBlurVertex.cso");
	device->CreateVertexShader(vsBlob.data(), vsBlob.size(), nullptr, m_yBlurVertex.ReleaseAndGetAddressOf());

	auto psBlob = DX::ReadData(L"BlurPixel.cso");
	device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_blurPixel.ReleaseAndGetAddressOf());
	psBlob = DX::ReadData(L"BloomPixel.cso");
	device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_bloomPixel.ReleaseAndGetAddressOf());
	SetCurrentDirectory(L"../../");
}

//ブルーム描画
void GameObjectManager::DrawBloom(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
	auto context = DeviceAccessor::GetInstance()->GetContext();
	auto depthStencil = dsv;
	float width = DeviceAccessor::GetInstance()->GetScreenSize().right;
	float height = DeviceAccessor::GetInstance()->GetScreenSize().bottom;
	//出力した輝度にガウシアンブラーをかけて4回ダウンサンプリングする
	for (int i = 0;i < 4;i++)
	{
		//横ブラー用に解像度を半分にする
		width /= 2.f;
		for (int j = 0;j < 2;j++)
		{
			context->ClearRenderTargetView(m_blurRTV[i][j].Get(), Colors::Black);

			//レンダーターゲット設定
			context->OMSetRenderTargets(1, m_blurRTV[i][j].GetAddressOf(), nullptr);
			CD3D11_VIEWPORT view(0.f, 0.f, float(width), float(height));
			context->RSSetViewports(1, &view);

			context->IASetInputLayout(m_inputLayout.Get());
			
			if (j == 0)
			{
				//縦ブラー用に解像度を半分にする
				height /= 2.f;

				//横ブラー用の頂点シェーダー設定
				context->VSSetShader(m_xBlurVertex.Get(), nullptr, 0);
			}
			else
			{
				//縦ブラー用頂点シェーダー設定
				context->VSSetShader(m_yBlurVertex.Get(), nullptr, 0);
			}
			//ピクセルシェーダー設定
			context->PSSetShader(m_blurPixel.Get(), nullptr, 0);
			//SRV設定
			if (i == 0 && j == 0)
			{
				context->VSSetShaderResources(0, 1, m_bloomSRV.GetAddressOf());
				context->PSSetShaderResources(0, 1, m_bloomSRV.GetAddressOf());
			}
			else if (i == 0)
			{
				context->VSSetShaderResources(0, 1, m_blurSRV[i][j - 1].GetAddressOf());
				context->PSSetShaderResources(0, 1, m_blurSRV[i][j - 1].GetAddressOf());
			}
			else if (j == 0)
			{
				context->VSSetShaderResources(0, 1, m_blurSRV[i - 1][j].GetAddressOf());
				context->PSSetShaderResources(0, 1, m_blurSRV[i - 1][j].GetAddressOf());
			}
			else
			{
				context->VSSetShaderResources(0, 1, m_blurSRV[i - 1][j - 1].GetAddressOf());
				context->PSSetShaderResources(0, 1, m_blurSRV[i - 1][j - 1].GetAddressOf());
			}
			auto states = DeviceAccessor::GetInstance()->GetStates();
			ID3D11SamplerState* samplers[] =
			{
				states->LinearClamp(),
				states->LinearClamp(),
			};
			context->PSSetSamplers(0, 1, samplers);

			context->OMSetBlendState(states->Opaque(), 0, 0xffffffff);
			context->OMSetDepthStencilState(states->DepthDefault(), 0);

			auto bb = m_blurBuffer.GetBuffer();
			context->PSSetConstantBuffers(5, 1, &bb);

			UINT stride = sizeof(Vertex);
			UINT offset = 0;
			context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
			context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->DrawIndexed(6, 0, 0);
		}
	}

	//川瀬式ブルームフィルターの適用
	width = DeviceAccessor::GetInstance()->GetScreenSize().right;
	height = DeviceAccessor::GetInstance()->GetScreenSize().bottom;
	context->OMSetRenderTargets(1, &rtv, depthStencil);
	CD3D11_VIEWPORT view(0.f, 0.f, float(width), float(height));
	context->RSSetViewports(1, &view);

	context->IASetInputLayout(m_inputLayout.Get());
	context->VSSetShader(m_PostProccessVertex.Get(), nullptr, 0);
	context->PSSetShader(m_bloomPixel.Get(), nullptr, 0);
	context->PSSetShaderResources(0, 1, m_blurSRV[0][1].GetAddressOf());
	context->PSSetShaderResources(1, 1, m_blurSRV[1][1].GetAddressOf());
	context->PSSetShaderResources(2, 1, m_blurSRV[2][1].GetAddressOf());
	context->PSSetShaderResources(3, 1, m_blurSRV[3][1].GetAddressOf());

	auto states = DeviceAccessor::GetInstance()->GetStates();
	ID3D11SamplerState* samplers[] =
	{
		states->LinearClamp(),
		states->LinearClamp(),
	};
	context->PSSetSamplers(0, 1, samplers);

	context->OMSetBlendState(states->Additive(), 0, 0xffffffff);
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->DrawIndexed(6, 0, 0);
}