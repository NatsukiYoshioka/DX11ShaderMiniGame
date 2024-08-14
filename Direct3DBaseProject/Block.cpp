#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
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

	//モデルのロード
	SetCurrentDirectory(L"Assets/Models");
	m_modelHandle = Model::CreateFromSDKMESH(deviceAccessor->GetDevice(),
		fileName,
		*deviceAccessor->GetEffectFactory(),
		ModelLoader_Clockwise);
	SetCurrentDirectory(L"../../");

	//エフェクトの初期化
	SetCurrentDirectory(L"Assets/Shader");
	m_effect = make_shared<OriginalEffect>(deviceAccessor->GetDevice());
	SetCurrentDirectory(L"../../");

	//モデルの各メッシュの描画方法の設定
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
			part->CreateInputLayout(deviceAccessor->GetDevice(), m_effect.get(), m_inputLayout.ReleaseAndGetAddressOf());
			part->ModifyEffect(deviceAccessor->GetDevice(), m_effect, false);
		}
	}

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

	//初期座標とY軸回転量の設定
	m_pos = pos;
	m_rotate = XM_PI / rotate;
}

//データ破棄
Block::~Block()
{
	m_modelHandle.reset();
}

//オブジェクトの更新
void Block::Update()
{
	//ワールド座標行列の設定
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(0.01f));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationY(m_rotate));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));
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