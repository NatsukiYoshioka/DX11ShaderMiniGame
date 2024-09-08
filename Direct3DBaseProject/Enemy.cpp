#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"Desk.h"
#include"DeskAccessor.h"
#include"OriginalEffect.h"
#include<random>
#define _USE_MATH_DEFINES
#include<math.h>
#include "Enemy.h"

//オブジェクト初期化
Enemy::Enemy(const wchar_t* fileName, Vector3 pos, float rotate):
	m_nowAnimationState(AnimationState::Idle),
	m_posAngle(0),
	m_eyeDirection(Vector3::Zero),
	m_eyeView(),
	m_isStartMoving(false),
	m_scale(float(Json::GetInstance()->GetData()["EnemyScale"])),
	m_distance(float(Json::GetInstance()->GetData()["EnemyDistance"])),
	m_speed(float(Json::GetInstance()->GetData()["EnemySpeed"]))
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
	SetCurrentDirectory(L"Assets/Shader");
	m_effect = make_unique<OriginalEffect>(deviceAccessor->GetDevice(), OriginalEffect::PixelType::Character, true);
	SetCurrentDirectory(L"../../");

	//モデルの各メッシュの描画設定
	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);


		if (mesh->boneIndex != ModelBone::c_Invalid)
		{
			for (const auto& pit : mesh->meshParts)
			{
				auto part = pit.get();
				assert(part != nullptr);

				//入力レイアウトの初期化とエフェクトの適用
				part->ModifyEffect(deviceAccessor->GetDevice(), m_effect, false);
			}
		}
	}
	m_modelHandle->Modified();

	//アニメーションの設定
	SetCurrentDirectory(L"Assets/Models");
	auto json = Json::GetInstance();
	m_animations.resize(json->GetData()["Enemy"].size() - 1);
	for (int i = 0; i < json->GetData()["Enemy"].size() - 1; i++)
	{
		DX::ThrowIfFailed(m_animations.at(i).Load(json->Widen(json->GetData()["Enemy"].at(i + 1)).c_str()));
		m_animations.at(i).Bind(*m_modelHandle);
	}
	SetCurrentDirectory(L"../../");

	auto nbones = m_modelHandle->bones.size();
	m_drawBones = ModelBone::MakeArray(nbones);

	//テクスチャのロード
	ComPtr<ID3D11ShaderResourceView> texture;
	SetCurrentDirectory(L"Assets/Models");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["EnemyTexture"]).c_str(),
		nullptr,
		texture.ReleaseAndGetAddressOf()));
	SetCurrentDirectory(L"../../");

	//テクスチャの設定
	m_effect->SetTexture(texture.Get());

	//座標とY軸回転量の設定
	m_pos = pos;
	m_eyePos = m_pos;
	m_eyePos.y = float(json->GetData()["EnemyEyeHeight"]);
	m_rotate = rotate * XM_PI / 180.f;
}

//データ破棄
Enemy::~Enemy()
{
	m_modelHandle.reset();
}

//オブジェクト更新
void Enemy::Update()
{
	std::random_device random;
	std::mt19937 generator(random());
	using dist_type = std::uniform_int_distribution<int>;
	dist_type distribution(0, 99);
	dist_type::param_type animationParam(0, 2);

	//プレイヤーがスタート位置から離れたら動き始める
	if (Json::GetInstance()->GetData()["EnemyStartMovingPlayerPosition"].at(0) < PlayerAccessor::GetInstance()->GetPlayer()->GetPos().x)
	{
		m_isStartMoving = true;
	}
	if (m_isStartMoving)
	{
		//ランダムで敵の挙動を決定
		switch (distribution(generator))
		{
		case 0:
			distribution.param(animationParam);
			switch (distribution(generator))
			{
			case 0:
				m_nowAnimationState = AnimationState::Idle;
				break;
			case 1:
				m_nowAnimationState = AnimationState::RightWalk;
				break;
			case 2:
				m_nowAnimationState = AnimationState::LeftWalk;
				break;
			}
			break;
		default:
			break;
		}
	}

	if (m_nowAnimationState == AnimationState::LeftWalk)
	{
		m_posAngle += m_speed;
	}
	else if (m_nowAnimationState == AnimationState::RightWalk)
	{
		m_posAngle -= m_speed;
	}

	//向きと座標の計算
	auto deskPos = DeskAccessor::GetInstance()->GetDesk()->GetPos();
	float radian = m_posAngle * XM_PI / 180;
	float x = deskPos.x + sin(radian) * m_distance;
	float z = deskPos.z + cos(radian) * m_distance;
	m_pos.x = x;
	m_pos.z = z;

	x = deskPos.x + sin(radian) * (m_distance);
	z = deskPos.z + cos(radian) * (m_distance);
	m_eyePos.x = x;
	m_eyePos.z = z;

	auto playerPos = PlayerAccessor::GetInstance()->GetPlayer()->GetPos();
	m_rotate = atan2f(m_pos.x - playerPos.x, m_pos.z - playerPos.z);

	//敵視点のビュー空間行列生成
	m_eyeView = Matrix::CreateLookAt(m_eyePos, playerPos, Vector3::Up);

	m_eyeDirection = playerPos - m_eyePos;
	m_eyeDirection.Normalize();

	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);
		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			auto effect = static_cast<OriginalEffect*>(part->effect.get());
			effect->SetLightPosition(m_eyePos);
			effect->SetLightDirection(m_eyeDirection);
			effect->SetEyePosition(CameraAccessor::GetInstance()->GetCamera()->GetPos());
			effect->SetLightView(m_eyeView);
		}
	}

	//ワールド座標行列の更新
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(m_scale));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationY(m_rotate));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));

	m_animations.at(static_cast<int>(m_nowAnimationState)).Update(*DeviceAccessor::GetInstance()->GetElapsedTime());
}

//オブジェクト描画
void Enemy::Draw()
{
	size_t nbones = m_modelHandle->bones.size();

	m_animations.at(static_cast<int>(m_nowAnimationState)).Apply(*m_modelHandle, nbones, m_drawBones.get());

	m_modelHandle->DrawSkinned(DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		nbones,
		m_drawBones.get(),
		m_world,
		CameraAccessor::GetInstance()->GetCamera()->GetView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection());
}