#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"OriginalEffect.h"
#include"Enemy.h"
#include"EnemyAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include "Room.h"

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

	//エフェクトの初期化
	/*SetCurrentDirectory(L"Assets/Shader");
	m_effect.assign(m_modelHandle->meshes.size(), 
		make_unique<OriginalEffect>(deviceAccessor->GetDevice(), OriginalEffect::PixelType::Object));
	SetCurrentDirectory(L"../../");*/

	//モデルの各メッシュの描画設定
	//int index = 0;
	//for (const auto& mit : m_modelHandle->meshes)
	//{
	//	auto mesh = mit.get();
	//	assert(mesh != nullptr);

	//	if (mesh->boneIndex != ModelBone::c_Invalid)
	//	{
	//		for (const auto& pit : mesh->meshParts)
	//		{
	//			auto part = pit.get();
	//			assert(part != nullptr);

	//			//入力レイアウトの初期化とエフェクトの適用
	//			part->ModifyEffect(deviceAccessor->GetDevice(), m_effect.at(index), false);
	//		}
	//	}
	//	index++;
	//}
	//m_modelHandle->Modified();

	auto json = Json::GetInstance();
	int index = 0;
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

Room::~Room()
{
	m_modelHandle.reset();
}

void Room::InitializeTitle()
{
	
}

void Room::UpdateTitle()
{
	Update();
}

void Room::DrawTitle()
{
	Draw();
}

void Room::Initialize()
{

}

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
		}
	}
}

void Room::Draw()
{
	m_modelHandle->Draw(DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		m_world,
		CameraAccessor::GetInstance()->GetCamera()->GetView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection());
}

void Room::InitializeResult()
{

}

void Room::UpdateResult()
{

}

void Room::DrawResult()
{

}

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