#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"Enemy.h"
#include"EnemyAccessor.h"
#include"OriginalEffect.h"
#include"GameObject.h"
#include "Player.h"

extern void ExitGame() noexcept;

Player::Player(const wchar_t* fileName, Vector3 pos, float rotate):
	m_nowAnimationState(AnimationState::Idle),
	m_scale(float(Json::GetInstance()->GetData()["PlayerScale"])),
	m_speed(float(Json::GetInstance()->GetData()["PlayerSpeed"])),
	m_runSpeed(float(Json::GetInstance()->GetData()["PlayerRunSpeed"])),
	m_crouchSpeed(float(Json::GetInstance()->GetData()["PlayerCrouchSpeed"]))
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
	m_animations.resize(json->GetData()["Player"].size() - 1);
	for (int i = 0; i < json->GetData()["Player"].size() - 1; i++)
	{
		DX::ThrowIfFailed(m_animations.at(i).Load(json->Widen(json->GetData()["Player"].at(i + 1)).c_str()));
		m_animations.at(i).Bind(*m_modelHandle);
	}
	SetCurrentDirectory(L"../../");

	auto nbones = m_modelHandle->bones.size();
	m_drawBones = ModelBone::MakeArray(nbones);

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

	//座標とY軸回転量の設定
	m_pos = pos;
	m_rotate = rotate * XM_PI / 180.f;
}

Player::~Player()
{
	m_modelHandle.reset();
}

void Player::Update()
{
	auto pad = DeviceAccessor::GetInstance()->GetGamePad()->GetState(0);

	
	bool isMove = false;
	bool isCrouch = false;
	float nowSpeed = 0.f;
	m_nowAnimationState = AnimationState::Idle;
	if (pad.IsConnected())
	{
		if (pad.IsViewPressed())
		{
			ExitGame();
		}
		if (pad.IsLeftShoulderPressed())
		{
			m_nowAnimationState = AnimationState::Crouch;
			isCrouch = true;
		}
		if (pad.thumbSticks.leftX != 0 || pad.thumbSticks.leftY != 0)
		{
			m_rotate = atan2f(-pad.thumbSticks.leftX, pad.thumbSticks.leftY);
			
			isMove = true;
			nowSpeed = m_speed;
			m_nowAnimationState = AnimationState::Walk;

			if (isCrouch)
			{
				nowSpeed = m_crouchSpeed;
				m_nowAnimationState = AnimationState::CrouchedWalk;
			}
			else if (pad.IsRightShoulderPressed())
			{
				nowSpeed = m_runSpeed;
				m_nowAnimationState = AnimationState::Run;
			}
		}
	}

	if (isMove)
	{
		auto cameraPitch = CameraAccessor::GetInstance()->GetCamera()->GetPitch();
		auto radian = cameraPitch * XM_PI / 180;
		m_rotate += radian;

		m_pos.z += -cos(m_rotate) * nowSpeed;
		m_pos.x += -sin(m_rotate) * nowSpeed;
	}

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
		}
	}

	//ワールド座標行列の更新
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(m_scale));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationY(m_rotate));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));

	m_animations.at(static_cast<int>(m_nowAnimationState)).Update(*DeviceAccessor::GetInstance()->GetElapsedTime());
}

void Player::Draw()
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