#include "pch.h"
#include"DeviceAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"GameObject.h"
#include<WinBase.h>
#include "Block.h"

Block::Block(const wchar_t* fileName, Vector3 pos, float rotate)
{
	auto deviceAccessor = DeviceAccessor::GetInstance();
	SetCurrentDirectory(L"Assets/Models");
	m_modelHandle = Model::CreateFromCMO(deviceAccessor->GetDevice(),
		fileName,
		*deviceAccessor->GetEffectFactory(),
		ModelLoader_Clockwise);
	m_pos = pos;
	m_rotate = XM_PI / rotate;
}

Block::~Block()
{
	m_modelHandle.reset();
}

void Block::Update()
{
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationY(m_rotate));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));
}

void Block::Draw()
{
	m_modelHandle->Draw(
		DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		m_world,
		CameraAccessor::GetInstance()->GetCamera()->GetView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection());
}