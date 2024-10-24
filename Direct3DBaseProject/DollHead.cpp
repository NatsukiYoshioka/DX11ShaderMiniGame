#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"OriginalEffect.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"Enemy.h"
#include"EnemyAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"FoundUI.h"
#include"UIBase.h"
#include"UIAccessor.h"
#include"GameObjectManager.h"
#include "DollHead.h"

//オブジェクトの初期化
DollHead::DollHead(const wchar_t* fileName, Vector3 pos):
	m_firstPos(pos),
	m_finalPos(Vector3(Json::GetInstance()->GetData()["PlayerHeadFinalPos"].at(0),
		Json::GetInstance()->GetData()["PlayerHeadFinalPos"].at(1),
		Json::GetInstance()->GetData()["PlayerHeadFinalPos"].at(2))),
	m_posRatio(0),
	m_speed(),
	m_firstSpeed(float(Json::GetInstance()->GetData()["PlayerHeadFirstSpeed"])),
	m_subSpeed(float(Json::GetInstance()->GetData()["PlayerHeadSubSpeed"])),
	m_rotate(),
	m_rotationSpeed(),
	m_firstRotationSpeed(float(Json::GetInstance()->GetData()["PlayerHeadRotationFirstSpeed"])),
	m_subRotationSpeed(float(Json::GetInstance()->GetData()["PlayerHeadRotationSubSpeed"]))
{
	auto deviceAccessor = DeviceAccessor::GetInstance();
	//モデルのロード
	SetCurrentDirectory(L"Assets/Models");
	m_modelHandle = Model::CreateFromSDKMESH(deviceAccessor->GetDevice(),
		fileName,
		*deviceAccessor->GetEffectFactory(),
		ModelLoader_Clockwise);
	SetCurrentDirectory(L"../../");

	//シェーダーの初期化
	SetCurrentDirectory(L"Assets/Shader");
	m_effect = make_unique<OriginalEffect>(deviceAccessor->GetDevice(), OriginalEffect::PixelType::Character);
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

	auto json = Json::GetInstance();
	//テクスチャのロード
	ComPtr<ID3D11ShaderResourceView> texture;
	SetCurrentDirectory(L"Assets/Models");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["PlayerTexture"]).c_str(),
		nullptr,
		texture.ReleaseAndGetAddressOf()));
	SetCurrentDirectory(L"../../");

	//テクスチャの設定
	m_effect->SetTexture(texture.Get());

	m_pos = pos;
}

//オブジェクトの破棄
DollHead::~DollHead()
{
	m_modelHandle.reset();
}

//タイトルでの初期化(処理なし)
void DollHead::InitializeTitle()
{

}

//タイトルでの更新(処理なし)
void DollHead::UpdateTitle()
{

}

//タイトルでの描画(処理なし)
void DollHead::DrawTitle()
{

}

//ゲームシーンの初期化(処理なし)
void DollHead::Initialize()
{

}

//ゲームシーンの更新(処理なし)
void DollHead::Update()
{

}

//ゲームシーンの描画(処理なし)
void DollHead::Draw()
{

}

//リザルトシーンでの初期化
void DollHead::InitializeResult()
{
	m_speed = m_firstSpeed;
	m_rotationSpeed = m_firstRotationSpeed;
	m_pos = m_firstPos;
	m_rotate = float(Json::GetInstance()->GetData()["PlayerHeadFirstRotate"]);
}

//リザルトシーンでの更新
void DollHead::UpdateResult()
{
	//ゲームオーバー時に処理を続ける
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())return;
	//シェーダー描画時の情報取得
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

	//移動限界の処理
	if (m_pos.x > m_finalPos.x && m_speed > 0)
	{
		m_speed -= m_subSpeed;
		m_pos.x -= m_speed;
		m_rotationSpeed -= m_subRotationSpeed;
		m_rotate += m_rotationSpeed;
		m_posRatio = (m_pos.x - m_firstPos.x) / (m_finalPos.x - m_firstPos.x);
	}

	//ワールド座標行列の更新
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(0.05f));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationX(90.f * XM_PI / 180.f));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationZ(m_rotate * XM_PI / 180.f));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));
}

//リザルトでの描画
void DollHead::DrawResult()
{
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())return;
	m_modelHandle->Draw(DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		m_world,
		CameraAccessor::GetInstance()->GetCamera()->GetView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection());
}

void DollHead::DrawShadow()
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
			effect->UpdateType(OriginalEffect::PixelType::Character);
		}
	}
}