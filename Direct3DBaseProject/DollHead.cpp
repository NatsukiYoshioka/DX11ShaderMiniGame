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

//�I�u�W�F�N�g�̏�����
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
	//���f���̃��[�h
	SetCurrentDirectory(L"Assets/Models");
	m_modelHandle = Model::CreateFromSDKMESH(deviceAccessor->GetDevice(),
		fileName,
		*deviceAccessor->GetEffectFactory(),
		ModelLoader_Clockwise);
	SetCurrentDirectory(L"../../");

	//�V�F�[�_�[�̏�����
	SetCurrentDirectory(L"Assets/Shader");
	m_effect = make_unique<OriginalEffect>(deviceAccessor->GetDevice(), OriginalEffect::PixelType::Character);
	SetCurrentDirectory(L"../../");

	//���f���̊e���b�V���̕`��ݒ�
	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);

		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			//���̓��C�A�E�g�̏������ƃG�t�F�N�g�̓K�p
			part->ModifyEffect(deviceAccessor->GetDevice(), m_effect, false);
		}
	}
	m_modelHandle->Modified();

	auto json = Json::GetInstance();
	//�e�N�X�`���̃��[�h
	ComPtr<ID3D11ShaderResourceView> texture;
	SetCurrentDirectory(L"Assets/Models");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["PlayerTexture"]).c_str(),
		nullptr,
		texture.ReleaseAndGetAddressOf()));
	SetCurrentDirectory(L"../../");

	//�e�N�X�`���̐ݒ�
	m_effect->SetTexture(texture.Get());

	m_pos = pos;
}

//�I�u�W�F�N�g�̔j��
DollHead::~DollHead()
{
	m_modelHandle.reset();
}

//�^�C�g���ł̏�����(�����Ȃ�)
void DollHead::InitializeTitle()
{

}

//�^�C�g���ł̍X�V(�����Ȃ�)
void DollHead::UpdateTitle()
{

}

//�^�C�g���ł̕`��(�����Ȃ�)
void DollHead::DrawTitle()
{

}

//�Q�[���V�[���̏�����(�����Ȃ�)
void DollHead::Initialize()
{

}

//�Q�[���V�[���̍X�V(�����Ȃ�)
void DollHead::Update()
{

}

//�Q�[���V�[���̕`��(�����Ȃ�)
void DollHead::Draw()
{

}

//���U���g�V�[���ł̏�����
void DollHead::InitializeResult()
{
	m_speed = m_firstSpeed;
	m_rotationSpeed = m_firstRotationSpeed;
	m_pos = m_firstPos;
	m_rotate = float(Json::GetInstance()->GetData()["PlayerHeadFirstRotate"]);
}

//���U���g�V�[���ł̍X�V
void DollHead::UpdateResult()
{
	//�Q�[���I�[�o�[���ɏ����𑱂���
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())return;
	//�V�F�[�_�[�`�掞�̏��擾
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

	//�ړ����E�̏���
	if (m_pos.x > m_finalPos.x && m_speed > 0)
	{
		m_speed -= m_subSpeed;
		m_pos.x -= m_speed;
		m_rotationSpeed -= m_subRotationSpeed;
		m_rotate += m_rotationSpeed;
		m_posRatio = (m_pos.x - m_firstPos.x) / (m_finalPos.x - m_firstPos.x);
	}

	//���[���h���W�s��̍X�V
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(0.05f));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationX(90.f * XM_PI / 180.f));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationZ(m_rotate * XM_PI / 180.f));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));
}

//���U���g�ł̕`��
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
	//�V�F�[�_�[���e�p�ɕύX
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

	//�`��
	m_modelHandle->Draw(DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		m_world,
		EnemyAccessor::GetInstance()->GetEnemy()->GetEyeView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection());

	//�V�F�[�_�[�����ɖ߂�
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