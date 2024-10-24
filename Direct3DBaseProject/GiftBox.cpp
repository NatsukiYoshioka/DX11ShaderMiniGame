#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Enemy.h"
#include"EnemyAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"GameObject.h"
#include"OriginalEffect.h"
#include"FoundUI.h"
#include"UIBase.h"
#include"UIAccessor.h"
#include"GameObjectManager.h"
#include "GiftBox.h"

//オブジェクトの初期化
GiftBox::GiftBox(const wchar_t* fileName, Vector3 pos, float rotate)
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
	m_effect = make_unique<OriginalEffect>(deviceAccessor->GetDevice(), OriginalEffect::PixelType::Object);
	SetCurrentDirectory(L"../../");

	//モデルの各メッシュの描画設定
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
		}
	}

	//テクスチャのロード
	auto json = Json::GetInstance();
	ComPtr<ID3D11ShaderResourceView> texture;
	ComPtr<ID3D11ShaderResourceView> normal;
	SetCurrentDirectory(L"Assets/Models");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["GiftBoxTexture"].at(0)).c_str(),
		nullptr,
		texture.ReleaseAndGetAddressOf()));
	SetCurrentDirectory(L"../../");

	//テクスチャの設定
	m_effect->SetTexture(texture.Get());

	//座標とY軸回転量の設定
	m_pos = pos;
	m_rotate = rotate * XM_PI / 180.f;

	//ワールド座標行列の更新
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(0.5f));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationY(m_rotate));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));
}

//データ破棄
GiftBox::~GiftBox()
{

}

//タイトルでの初期化(処理なし)
void GiftBox::InitializeTitle()
{

}

//タイトルでの更新
void GiftBox::UpdateTitle()
{
	//シェーダー用の設定
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
			for (int i = 0; i < UIAccessor::GetInstance()->GetUIs().size(); i++)
			{
				auto foundUI = dynamic_cast<FoundUI*>(UIAccessor::GetInstance()->GetUIs().at(i));
				if (foundUI)
				{
					//effect->SetLightColor(Vector3(1.f, 1.f - foundUI->GetTimeRatio(), 1.f - foundUI->GetTimeRatio()));
					effect->SetLightColor(Vector3(1.f, 1.f, 1.f));
					break;
				}
			}
		}
	}
}

//タイトルでの描画
void GiftBox::DrawTitle()
{
	m_modelHandle->Draw(DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		m_world,
		CameraAccessor::GetInstance()->GetCamera()->GetView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection());
}

//ゲームシーンでの初期化(処理なし)
void GiftBox::Initialize()
{

}

//ゲームシーンでの更新(処理なし)
void GiftBox::Update()
{

}

//ゲームシーンでの描画(処理なし)
void GiftBox::Draw()
{

}

//リザルトでの初期化(処理なし)
void GiftBox::InitializeResult()
{

}

//リザルトでの更新(処理なし)
void GiftBox::UpdateResult()
{

}

//リザルトでの描画(処理なし)
void GiftBox::DrawResult()
{

}

//影の描画
void GiftBox::DrawShadow()
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