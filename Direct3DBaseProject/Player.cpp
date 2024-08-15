#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"OriginalEffect.h"
#include"GameObject.h"
#include "Player.h"

extern void ExitGame() noexcept;

Player::Player(const wchar_t* fileName, Vector3 pos, float rotate):
	m_nowAnimationState(AnimationState::Idle),
	m_speed(float(Json::GetInstance()->GetData()["PlayerSpeed"]))
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
	m_effect = make_unique<OriginalEffect>(deviceAccessor->GetDevice(), true);
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
	m_animations.resize(json->GetData()["Player"].size() - 1);
	for (int i = 0; i < json->GetData()["Player"].size() - 1; i++)
	{
		DX::ThrowIfFailed(m_animations.at(i).Load(json->Widen(json->GetData()["Player"].at(i + 1)).c_str()));
		m_animations.at(i).Bind(*m_modelHandle);
	}
	SetCurrentDirectory(L"../../");

	auto nbones = m_modelHandle->bones.size();
	m_drawBones = ModelBone::MakeArray(nbones);

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

	//���W��Y����]�ʂ̐ݒ�
	m_pos = pos;
	m_rotate = XM_PI / rotate;
}

Player::~Player()
{
	m_modelHandle.reset();
}

void Player::Update()
{
	auto pad = DeviceAccessor::GetInstance()->GetGamePad()->GetState(0);

	
	bool isMove = false;
	m_nowAnimationState = AnimationState::Idle;
	if (pad.IsConnected())
	{
		if (pad.IsViewPressed())
		{
			ExitGame();
		}
		if (pad.thumbSticks.leftX != 0 || pad.thumbSticks.leftY != 0)
		{
			m_rotate = atan2f(-pad.thumbSticks.leftX, pad.thumbSticks.leftY);
			
			isMove = true;
			m_nowAnimationState = AnimationState::Walk;
		}
	}

	if (isMove)
	{
		auto cameraPitch = CameraAccessor::GetInstance()->GetCamera()->GetPitch();
		auto radian = cameraPitch * XM_PI / 180;
		m_rotate += radian;

		m_pos.z += -cos(m_rotate) * m_speed;
		m_pos.x += -sin(m_rotate) * m_speed;
	}

	//���[���h���W�s��̍X�V
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(0.0075f));
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