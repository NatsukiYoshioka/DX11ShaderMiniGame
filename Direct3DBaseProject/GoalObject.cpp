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
	//���f���̃��[�h
	SetCurrentDirectory(L"Assets/Models");
	m_modelHandle = Model::CreateFromSDKMESH(deviceAccessor->GetDevice(),
		fileName,
		*deviceAccessor->GetEffectFactory(),
		ModelLoader_Clockwise);
	SetCurrentDirectory(L"../../");

	SetCurrentDirectory(L"Assets/Shader");
	m_effect = make_unique<OriginalEffect>(deviceAccessor->GetDevice(), OriginalEffect::PixelType::Light);
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
	
	m_pos = pos;
	m_pos.z += m_fixPosZ;
}

GoalObject::~GoalObject()
{

}

//�^�C�g���ł̏�����(�����Ȃ�)
void GoalObject::InitializeTitle()
{

}

//�^�C�g���ł̍X�V(�����Ȃ�)
void GoalObject::UpdateTitle()
{

}

//�^�C�g���ł̕`��(�����Ȃ�)
void GoalObject::DrawTitle()
{
	
}

//�Q�[���V�[���̏�����(�����Ȃ�)
void GoalObject::Initialize()
{

}

//�Q�[���V�[���̍X�V(�����Ȃ�)
void GoalObject::Update()
{
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, XMMatrixScalingFromVector(m_scale));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));

	//�V�F�[�_�[�p�̐ݒ�
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

//�Q�[���V�[���̕`��
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

//���U���g�V�[���ł̏�����
void GoalObject::InitializeResult()
{
	
}

//���U���g�V�[���ł̍X�V
void GoalObject::UpdateResult()
{
	
}

//���U���g�ł̕`��
void GoalObject::DrawResult()
{
	
}

//�e�̕`��(�����Ȃ�)
void GoalObject::DrawShadow()
{
	
}