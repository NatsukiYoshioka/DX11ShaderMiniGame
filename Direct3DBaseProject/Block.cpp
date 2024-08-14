#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"GameObject.h"
#include"OriginalEffect.h"
#include<WinBase.h>
#include<random>
#define _USE_MATH_DEFINES
#include<math.h>
#include "Block.h"

//�ςݖ؃u���b�N�̏�����
Block::Block(const wchar_t* fileName, Vector3 pos, float rotate)
{
	auto deviceAccessor = DeviceAccessor::GetInstance();

	//���f���̃��[�h
	SetCurrentDirectory(L"Assets/Models");
	m_modelHandle = Model::CreateFromSDKMESH(deviceAccessor->GetDevice(),
		fileName,
		*deviceAccessor->GetEffectFactory(),
		ModelLoader_Clockwise);
	SetCurrentDirectory(L"../../");

	//�G�t�F�N�g�̏�����
	SetCurrentDirectory(L"Assets/Shader");
	m_effect = make_shared<OriginalEffect>(deviceAccessor->GetDevice());
	SetCurrentDirectory(L"../../");

	//���f���̊e���b�V���̕`����@�̐ݒ�
	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);

		mesh->PrepareForRendering(deviceAccessor->GetContext(), *deviceAccessor->GetStates());
		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			//���̓��C�A�E�g�̏������ƃG�t�F�N�g�̓K�p
			part->CreateInputLayout(deviceAccessor->GetDevice(), m_effect.get(), m_inputLayout.ReleaseAndGetAddressOf());
			part->ModifyEffect(deviceAccessor->GetDevice(), m_effect, false);
		}
	}

	//�e�N�X�`���̐ݒ�(�����_��)
	auto json = Json::GetInstance();
	std::random_device random;
	std::mt19937 generator(random());
	using dist_type = std::uniform_int_distribution<int>;
	dist_type distribution(0, json->GetData()["BlockTexture"].size() - 1);
	ComPtr<ID3D11ShaderResourceView> texture;
	SetCurrentDirectory(L"Assets/Models");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["BlockTexture"].at(distribution(generator))).c_str(),
		nullptr,
		texture.ReleaseAndGetAddressOf()));
	SetCurrentDirectory(L"../../");
	m_effect->SetTexture(texture.Get());

	//�������W��Y����]�ʂ̐ݒ�
	m_pos = pos;
	m_rotate = XM_PI / rotate;
}

//�f�[�^�j��
Block::~Block()
{
	m_modelHandle.reset();
}

//�I�u�W�F�N�g�̍X�V
void Block::Update()
{
	//���[���h���W�s��̐ݒ�
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(0.01f));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationY(m_rotate));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));
}

//�I�u�W�F�N�g�̕`��
void Block::Draw()
{
	m_modelHandle->Draw(DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		m_world,
		CameraAccessor::GetInstance()->GetCamera()->GetView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection());
}