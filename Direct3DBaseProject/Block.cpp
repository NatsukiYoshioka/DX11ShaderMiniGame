#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"Enemy.h"
#include"EnemyAccessor.h"
#include"GameObject.h"
#include"OriginalEffect.h"
#include<WinBase.h>
#include<random>
#define _USE_MATH_DEFINES
#include<math.h>
#include "Block.h"

//積み木ブロックの初期化
Block::Block(const wchar_t* fileName, Vector3 pos, float rotate)
{
	auto deviceAccessor = DeviceAccessor::GetInstance();

	//初期座標とY軸回転量の設定
	m_pos = pos;
	m_rotate = rotate * XM_PI / 180.f;

	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(0.0125f));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationY(m_rotate));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));

	//モデルのロード
	SetCurrentDirectory(L"Assets/Models");
	m_modelHandle = Model::CreateFromSDKMESH(deviceAccessor->GetDevice(),
		fileName,
		*deviceAccessor->GetEffectFactory(),
		ModelLoader_Clockwise);
	SetCurrentDirectory(L"../../");

	//エフェクトの初期化
	SetCurrentDirectory(L"Assets/Shader");
	m_effect = make_unique<OriginalEffect>(deviceAccessor->GetDevice(), OriginalEffect::PixelType::Object);
	SetCurrentDirectory(L"../../");

	//モデルの各メッシュの描画方法の設定
	VertexPositionNormalTexture* vertices;
	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);

		mesh->PrepareForRendering(deviceAccessor->GetContext(), *deviceAccessor->GetStates());
		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			//入力レイアウトの初期化とエフェクトの適用
			part->ModifyEffect(deviceAccessor->GetDevice(), m_effect, false);

			//頂点データとインデックスデータの取得
			D3D11_BUFFER_DESC vbDesc;
			part->vertexBuffer->GetDesc(&vbDesc);
			D3D11_BUFFER_DESC vbufferDesc = {};
			vbufferDesc.Usage = D3D11_USAGE_STAGING;
			vbufferDesc.ByteWidth = vbDesc.ByteWidth;
			vbufferDesc.BindFlags = 0;
			vbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			vbufferDesc.StructureByteStride = sizeof(VertexPositionNormalTexture);
			ID3D11Buffer* vBuffer;
			deviceAccessor->GetDevice()->CreateBuffer(&vbufferDesc, NULL, &vBuffer);
			deviceAccessor->GetContext()->CopyResource(vBuffer, part->vertexBuffer.Get());

			D3D11_MAPPED_SUBRESOURCE mapped;
			deviceAccessor->GetContext()->Map(vBuffer, 0, D3D11_MAP_READ, 0, &mapped);
			vertices = reinterpret_cast<VertexPositionNormalTexture*>(mapped.pData);
			deviceAccessor->GetContext()->Unmap(vBuffer, 0);

			//頂点データをワールド座標変換
			for (size_t i = 0;i < vbufferDesc.ByteWidth / vbufferDesc.StructureByteStride;i++)
			{
				XMVECTOR vertexPosition = XMLoadFloat3(&vertices[i].position);
				XMVECTOR worldPosition = XMVector3Transform(vertexPosition, m_world);
				XMFLOAT3 pos;
				XMStoreFloat3(&pos, worldPosition);
				m_vertices.push_back(pos);
			}
		}
	}
	m_modelHandle->Modified();

	//頂点バッファSRV作成
	auto device = DeviceAccessor::GetInstance()->GetDevice();
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * m_vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	vertexBufferDesc.StructureByteStride = sizeof(DirectX::XMFLOAT3);

	D3D11_SUBRESOURCE_DATA vertexInitData = {};
	vertexInitData.pSysMem = m_vertices.data();

	ID3D11Buffer* vertexBuffer;
	device->CreateBuffer(&vertexBufferDesc, &vertexInitData, &vertexBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = m_vertices.size();
	device->CreateShaderResourceView(vertexBuffer, &srvDesc, m_vertexBufferSRV.ReleaseAndGetAddressOf());

	//テクスチャの設定(ランダム)
	auto json = Json::GetInstance();
	std::random_device random;
	std::mt19937 generator(random());
	using dist_type = std::uniform_int_distribution<int>;
	dist_type distribution(0, json->GetData()["BlockTexture"].size() - 1);
	ComPtr<ID3D11ShaderResourceView> texture;
	SetCurrentDirectory(L"Assets/Models");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["BlockTexture"].at(distribution(generator))).c_str(),
		nullptr,
		texture.ReleaseAndGetAddressOf()));
	SetCurrentDirectory(L"../../");
	m_effect->SetTexture(texture.Get());
}

//データ破棄
Block::~Block()
{
	m_modelHandle.reset();
}

//タイトルシーンオブジェクトの初期化
void Block::InitializeTitle()
{

}

//タイトルシーンオブジェクトの更新
void Block::UpdateTitle()
{

}

//タイトルシーンオブジェクトの描画
void Block::DrawTitle()
{

}

//オブジェクトの初期化
void Block::Initialize()
{

}

//オブジェクトの更新
void Block::Update()
{
	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);
		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			auto effect = static_cast<OriginalEffect*>(part->effect.get());
			effect->SetLightPosition(EnemyAccessor::GetInstance()->GetEnemy()->GetEyePosition());
			effect->SetLightDirection(EnemyAccessor::GetInstance()->GetEnemy()->GetEyeDirection());
			effect->SetEyePosition(CameraAccessor::GetInstance()->GetCamera()->GetPos());
			effect->SetLightView(EnemyAccessor::GetInstance()->GetEnemy()->GetEyeView());
		}
	}
}

//オブジェクトの描画
void Block::Draw()
{
	m_modelHandle->Draw(DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		m_world,
		CameraAccessor::GetInstance()->GetCamera()->GetView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection());
}

//リザルトシーンオブジェクトの初期化
void Block::InitializeResult()
{

}

//リザルトシーンオブジェクトの更新
void Block::UpdateResult()
{
	
}

//リザルトシーンオブジェクトの描画
void Block::DrawResult()
{
	
}

//影の描画
void Block::DrawShadow()
{
	//シェーダーを影用に変更
	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);
		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			auto effect = static_cast<OriginalEffect*>(part->effect.get());
			effect->UpdateType(OriginalEffect::PixelType::Shadow);
		}
	}

	//描画
	m_modelHandle->Draw(DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		m_world,
		EnemyAccessor::GetInstance()->GetEnemy()->GetEyeView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection());

	//シェーダーを元に戻す
	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);
		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			auto effect = static_cast<OriginalEffect*>(part->effect.get());
			effect->UpdateType(OriginalEffect::PixelType::Object);
		}
	}
}

//敵視点での描画
void Block::DrawHitCheck()
{
	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);
		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			auto effect = static_cast<OriginalEffect*>(part->effect.get());
			effect->UpdateType(OriginalEffect::PixelType::Red);
		}
	}

	m_modelHandle->Draw(DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		m_world,
		EnemyAccessor::GetInstance()->GetEnemy()->GetEyeView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection());

	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);
		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			auto effect = static_cast<OriginalEffect*>(part->effect.get());
			effect->UpdateType(OriginalEffect::PixelType::Object);
		}
	}
}