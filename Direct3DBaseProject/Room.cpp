#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"OriginalEffect.h"
#include"Enemy.h"
#include"EnemyAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"FoundUI.h"
#include"UIBase.h"
#include"UIAccessor.h"
#include"GameObjectManager.h"
#include "Room.h"

//オブジェクトの初期化
Room::Room(const wchar_t* fileName)
{
	auto deviceAccessor = DeviceAccessor::GetInstance();

	//モデルのロード
	SetCurrentDirectory(L"Assets/Models");
	m_modelHandle = Model::CreateFromSDKMESH(deviceAccessor->GetDevice(),
		fileName,
		*deviceAccessor->GetEffectFactory(),
		ModelLoader_Clockwise | ModelLoader_IncludeBones);
	SetCurrentDirectory(L"../../");

	auto json = Json::GetInstance();
	int index = 0;
	//各メッシュのテクスチャ設定
	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);

		char c[1024];
		wcstombs(c, mesh->name.c_str(), sizeof(wchar_t) * int(mesh->name.size()));
		texture.push_back(NULL);
		SetCurrentDirectory(L"Assets/Models");
		CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
			json->Widen(json->GetData()[c]).c_str(),
			nullptr,
			texture.at(index).ReleaseAndGetAddressOf());
		SetCurrentDirectory(L"../../");
		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);
			shared_ptr<IEffect>& effect = part->effect;

			SetCurrentDirectory(L"Assets/Shader");
			effect = make_shared<OriginalEffect>(deviceAccessor->GetDevice(), OriginalEffect::PixelType::Object);
			SetCurrentDirectory(L"../../");
			part->CreateInputLayout(deviceAccessor->GetDevice(), effect.get(), part->inputLayout.ReleaseAndGetAddressOf());

			auto originalEffect = dynamic_pointer_cast<OriginalEffect>(part->effect);
			if (originalEffect)
			{
				originalEffect->SetTexture(texture.at(index).Get());
			}
			
		}
		index++;
	}
	
	//ワールド行列の設定
	m_pos = Vector3(
		float(json->GetData()["Room"].at(1).at(0)),
		float(json->GetData()["Room"].at(1).at(1)),
		float(json->GetData()["Room"].at(1).at(2)));
	m_rotate = float(json->GetData()["Room"].at(1).at(3));

	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(25.f));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationY(m_rotate));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));
}

//データ破棄
Room::~Room()
{
	m_modelHandle.reset();
}

//タイトルでの初期化(処理なし)
void Room::InitializeTitle()
{
	
}

//タイトルでの更新
void Room::UpdateTitle()
{
	Update();
}

//タイトルでの描画
void Room::DrawTitle()
{
	Draw();
}

//ゲームシーンでの初期化(処理なし)
void Room::Initialize()
{

}

//ゲームシーンでの更新
void Room::Update()
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

//ゲームシーンでの描画
void Room::Draw()
{
	m_modelHandle->Draw(DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		m_world,
		CameraAccessor::GetInstance()->GetCamera()->GetView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection());
}

//リザルトでの初期化(処理なし)
void Room::InitializeResult()
{

}

//リザルトでの更新
void Room::UpdateResult()
{
	Update();
}

//リザルトでの描画
void Room::DrawResult()
{
	Draw();
}

//影の描画
void Room::DrawShadow()
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