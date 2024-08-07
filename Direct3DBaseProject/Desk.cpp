#include "pch.h"
#include"Json.h"
#include"GameObject.h"
#include"OriginalEffect.h"
#include"DeviceAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include "Desk.h"

//デスクの初期化
Desk::Desk(const wchar_t* fileName, Vector3 pos, float rotate)
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
	m_effect = make_unique<OriginalEffect>(deviceAccessor->GetDevice());
	SetCurrentDirectory(L"../../");

	//モデルの各メッシュの描画設定
	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);

		mesh->PrepareForRendering(deviceAccessor->GetContext(), *deviceAccessor->GetStates());
	}

	//入力レイアウトの初期化
	CreateInputLayoutFromEffect(deviceAccessor->GetDevice(),
		m_effect.get(),
		deviceAccessor->GetInputElements(),
		deviceAccessor->GetInputElementSize(),
		m_inputLayout.ReleaseAndGetAddressOf());

	//テクスチャのロード
	auto json = Json::GetInstance();
	ComPtr<ID3D11ShaderResourceView> texture;
	ComPtr<ID3D11ShaderResourceView> normal;
	ComPtr<ID3D11ShaderResourceView> ao;
	SetCurrentDirectory(L"Assets/Models");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["DeskTexture"].at(0)).c_str(),
		nullptr,
		texture.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["DeskTexture"].at(1)).c_str(),
		nullptr,
		normal.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["DeskTexture"].at(2)).c_str(),
		nullptr,
		ao.ReleaseAndGetAddressOf()));
	SetCurrentDirectory(L"../../");

	//テクスチャの設定
	m_effect->SetTexture(texture.Get());
	m_effect->SetNormal(normal.Get());
	m_effect->SetAO(ao.Get());

	//座標とY軸回転量の設定
	m_pos = pos;
	m_rotate = XM_PI / rotate;

	//カメラプロジェクション行列の取得
	m_effect->SetProjection(CameraAccessor::GetInstance()->GetCamera()->GetProjection());
}

//データ破棄
Desk::~Desk()
{
	m_modelHandle.reset();
}

//オブジェクトの更新
void Desk::Update()
{
	//ワールド座標行列の更新
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(0.2f));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationY(m_rotate));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));

	//ワールド座標行列の取得
	m_effect->SetWorld(m_world);
	//カメラビュー行列の取得
	m_effect->SetView(CameraAccessor::GetInstance()->GetCamera()->GetView());
}

//オブジェクトの描画
void Desk::Draw()
{
	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);

		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			part->Draw(DeviceAccessor::GetInstance()->GetContext(),
				m_effect.get(),
				m_inputLayout.Get());
		}
	}
}