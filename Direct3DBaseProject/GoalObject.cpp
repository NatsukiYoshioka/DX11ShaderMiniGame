#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"OriginalEffect.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include "GoalObject.h"

GoalObject::GoalObject(const wchar_t* fileName, Vector3 pos, float rotate):
	m_world(),
	m_fixPosZ(float(Json::GetInstance()->GetData()["GoalBoxFixPosZ"])),
	m_scale(Vector3(
		Json::GetInstance()->GetData()["GoalBoxScale"].at(0),
		Json::GetInstance()->GetData()["GoalBoxScale"].at(1),
		Json::GetInstance()->GetData()["GoalBoxScale"].at(2)
	))
{
	auto deviceAccessor = DeviceAccessor::GetInstance();
	//モデルのロード
	SetCurrentDirectory(L"Assets/Models");
	m_modelHandle = Model::CreateFromSDKMESH(deviceAccessor->GetDevice(),
		fileName,
		*deviceAccessor->GetEffectFactory(),
		ModelLoader_Clockwise);
	SetCurrentDirectory(L"../../");

	SetCurrentDirectory(L"Assets/Shader");
	m_effect = make_unique<OriginalEffect>(deviceAccessor->GetDevice(), OriginalEffect::PixelType::Light);
	SetCurrentDirectory(L"../../");

	//モデルの各メッシュの描画設定
	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);

		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			//入力レイアウトの初期化とエフェクトの適用
			part->ModifyEffect(deviceAccessor->GetDevice(), m_effect, false);
		}
	}
	m_modelHandle->Modified();
	
	m_pos = pos;
	m_pos.z += m_fixPosZ;
}

GoalObject::~GoalObject()
{

}

//タイトルでの初期化(処理なし)
void GoalObject::InitializeTitle()
{

}

//タイトルでの更新(処理なし)
void GoalObject::UpdateTitle()
{

}

//タイトルでの描画(処理なし)
void GoalObject::DrawTitle()
{
	
}

//ゲームシーンの初期化(処理なし)
void GoalObject::Initialize()
{

}

//ゲームシーンの更新(処理なし)
void GoalObject::Update()
{
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, XMMatrixScalingFromVector(m_scale));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));

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
			effect->SetColor(Vector3(10.f, 10.f, 10.f));
		}
	}
}

//ゲームシーンの描画
void GoalObject::Draw()
{
	m_effect->Apply(DeviceAccessor::GetInstance()->GetContext());
	m_modelHandle->Draw(
		DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		m_world,
		CameraAccessor::GetInstance()->GetCamera()->GetView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection()
	);
}

//リザルトシーンでの初期化
void GoalObject::InitializeResult()
{
	
}

//リザルトシーンでの更新
void GoalObject::UpdateResult()
{
	
}

//リザルトでの描画
void GoalObject::DrawResult()
{
	
}

//影の描画(処理なし)
void GoalObject::DrawShadow()
{
	
}