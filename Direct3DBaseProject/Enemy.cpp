#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"DollHead.h"
#include"DollHeadAccessor.h"
#include"Desk.h"
#include"DeskAccessor.h"
#include"OriginalEffect.h"
#include<random>
#define _USE_MATH_DEFINES
#include<math.h>
#include "Enemy.h"

//�I�u�W�F�N�g������
Enemy::Enemy(const wchar_t* fileName):
	m_nowAnimationState(AnimationState::Idle),
	m_posAngle(0),
	m_eyeDirection(Vector3::Zero),
	m_eyeView(),
	m_isStartMoving(false),
	m_initializeTitlePos(Vector3(Json::GetInstance()->GetData()["EnemyTitlePosition"].at(0),
		Json::GetInstance()->GetData()["EnemyTitlePosition"].at(1),
		Json::GetInstance()->GetData()["EnemyTitlePosition"].at(2))),
	m_initializeGameClearPos(Vector3(Json::GetInstance()->GetData()["EnemyGameClearPosition"].at(0),
		Json::GetInstance()->GetData()["EnemyGameClearPosition"].at(1),
		Json::GetInstance()->GetData()["EnemyGameClearPosition"].at(2))),
	m_initializeGameOverPos(Vector3(Json::GetInstance()->GetData()["EnemyGameOverPosition"].at(0),
		Json::GetInstance()->GetData()["EnemyGameOverPosition"].at(1),
		Json::GetInstance()->GetData()["EnemyGameOverPosition"].at(2))),
	m_initializeTitleRotate(float(Json::GetInstance()->GetData()["EnemyTitlePosition"].at(3))* XM_PI / 180.f),
	m_initializeRotate(),
	m_scale(float(Json::GetInstance()->GetData()["EnemyScale"])),
	m_distance(float(Json::GetInstance()->GetData()["EnemyDistance"])),
	m_speed(float(Json::GetInstance()->GetData()["EnemySpeed"]))
{
	auto deviceAccessor = DeviceAccessor::GetInstance();

	//���f���̃��[�h
	SetCurrentDirectory(L"Assets/Models");
	m_modelHandle = Model::CreateFromSDKMESH(deviceAccessor->GetDevice(),
		fileName,
		*deviceAccessor->GetEffectFactory(),
		ModelLoader_Clockwise | ModelLoader_IncludeBones);
	SetCurrentDirectory(L"../../");

	//�G�t�F�N�g�̏�����
	SetCurrentDirectory(L"Assets/Shader");
	m_effect = make_unique<OriginalEffect>(deviceAccessor->GetDevice(), OriginalEffect::PixelType::Character, true);
	SetCurrentDirectory(L"../../");

	//���f���̊e���b�V���̕`��ݒ�
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

				//���̓��C�A�E�g�̏������ƃG�t�F�N�g�̓K�p
				part->ModifyEffect(deviceAccessor->GetDevice(), m_effect, false);
			}
		}
	}
	m_modelHandle->Modified();

	//�A�j���[�V�����̐ݒ�
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

	//�e�N�X�`���̃��[�h
	ComPtr<ID3D11ShaderResourceView> texture;
	SetCurrentDirectory(L"Assets/Models");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["EnemyTexture"]).c_str(),
		nullptr,
		texture.ReleaseAndGetAddressOf()));
	SetCurrentDirectory(L"../../");

	//�e�N�X�`���̐ݒ�
	m_effect->SetTexture(texture.Get());
}

//�f�[�^�j��
Enemy::~Enemy()
{
	m_modelHandle.reset();
}

//�^�C�g���V�[���I�u�W�F�N�g�̏�����
void Enemy::InitializeTitle()
{
	m_pos = m_initializeTitlePos;
	m_eyePos = m_pos;
	m_eyePos.y = float(Json::GetInstance()->GetData()["EnemyTitleEyeHeight"]);
	m_rotate = m_initializeTitleRotate;

	m_nowAnimationState = AnimationState::Idle;
	m_animations.at(static_cast<int>(AnimationState::PickUp)).ResetAnimTime();
}

//�^�C�g���V�[���I�u�W�F�N�g�̍X�V
void Enemy::UpdateTitle()
{
	//�����̌v�Z
	auto playerPos = PlayerAccessor::GetInstance()->GetPlayer()->GetPos();

	m_rotate = atan2f(m_pos.x - playerPos.x, m_pos.z - playerPos.z);

	//�G���_�̃r���[��ԍs�񐶐�
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

	//���[���h���W�s��̍X�V
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(m_scale));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationY(m_rotate));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));

	if (CameraAccessor::GetInstance()->GetCamera()->GetIsFinishMoving())
	{
		m_nowAnimationState = AnimationState::PickUp;
		m_animations.at(static_cast<int>(m_nowAnimationState)).Update(*DeviceAccessor::GetInstance()->GetElapsedTime() / 1.5);
	}
	else
	{
		m_animations.at(static_cast<int>(m_nowAnimationState)).Update(*DeviceAccessor::GetInstance()->GetElapsedTime());
	}
}

//�^�C�g���V�[���I�u�W�F�N�g�̕`��
void Enemy::DrawTitle()
{
	Draw();
}

//�I�u�W�F�N�g������
void Enemy::Initialize()
{
	//���W��Y����]�ʂ̐ݒ�
	m_nowAnimationState = AnimationState::Idle;
	m_eyePos = m_pos;
	m_eyePos.y = float(Json::GetInstance()->GetData()["EnemyEyeHeight"]);
	m_rotate = m_initializeRotate;
}

//�I�u�W�F�N�g�X�V
void Enemy::Update()
{
	std::random_device random;
	std::mt19937 generator(random());
	using dist_type = std::uniform_int_distribution<int>;
	dist_type distribution(0, 99);
	dist_type::param_type animationParam(0, 2);

	//�v���C���[���X�^�[�g�ʒu���痣�ꂽ�瓮���n�߂�
	if (Json::GetInstance()->GetData()["EnemyStartMovingPlayerPosition"].at(0) < PlayerAccessor::GetInstance()->GetPlayer()->GetPos().x)
	{
		m_isStartMoving = true;
	}
	if (m_isStartMoving)
	{
		//�����_���œG�̋���������
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

	//�����ƍ��W�̌v�Z
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

	//�G���_�̃r���[��ԍs�񐶐�
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

	//���[���h���W�s��̍X�V
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(m_scale));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationY(m_rotate));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));

	m_animations.at(static_cast<int>(m_nowAnimationState)).Update(*DeviceAccessor::GetInstance()->GetElapsedTime());
}

//�I�u�W�F�N�g�`��
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

//���U���g�V�[���I�u�W�F�N�g�̏�����
void Enemy::InitializeResult()
{
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())
	{
		m_nowAnimationState = AnimationState::Search;
		m_pos = m_initializeGameClearPos;
	}
	else
	{
		m_nowAnimationState = AnimationState::Idle;
		m_pos = m_initializeGameOverPos;
	}
	m_eyePos.x = m_pos.x;
	m_eyePos.z = m_pos.z;
}

//���U���g�V�[���I�u�W�F�N�g�̍X�V
void Enemy::UpdateResult()
{
	//�����̌v�Z
	auto deskPos = DeskAccessor::GetInstance()->GetDesk()->GetPos();
	auto dollHeadPos = DollHeadAccessor::GetInstance()->GetDollHead()->GetPos();

	m_rotate = atan2f(m_pos.x - deskPos.x, m_pos.z - deskPos.z);

	//�G���_�̃r���[��ԍs�񐶐�
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())
	{
		m_eyeView = Matrix::CreateLookAt(m_eyePos, deskPos, Vector3::Up);
		m_eyeDirection = deskPos - m_eyePos;
	}
	else
	{
		m_eyeView = Matrix::CreateLookAt(m_eyePos, dollHeadPos, Vector3::Up);
		m_eyeDirection = dollHeadPos - m_eyePos;
	}
	
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

	//���[���h���W�s��̍X�V
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(m_scale));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationY(m_rotate));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));

	m_animations.at(static_cast<int>(m_nowAnimationState)).Update(*DeviceAccessor::GetInstance()->GetElapsedTime());
}

//���U���g�V�[���I�u�W�F�N�g�̕`��
void Enemy::DrawResult()
{
	Draw();
}

void Enemy::DrawShadow()
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
			effect->UpdateType(OriginalEffect::PixelType::Shadow);
		}
	}

	size_t nbones = m_modelHandle->bones.size();

	m_animations.at(static_cast<int>(m_nowAnimationState)).Apply(*m_modelHandle, nbones, m_drawBones.get());

	m_modelHandle->DrawSkinned(DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		nbones,
		m_drawBones.get(),
		m_world,
		m_eyeView,
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
			effect->UpdateType(OriginalEffect::PixelType::Character);
		}
	}
}