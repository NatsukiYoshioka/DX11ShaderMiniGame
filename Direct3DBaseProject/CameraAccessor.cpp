#include "pch.h"
#include"Camera.h"
#include "CameraAccessor.h"

CameraAccessor* CameraAccessor::m_instance = nullptr;

//�A�N�Z�T�̏�����
CameraAccessor::CameraAccessor()
{
	m_camera = new Camera();
}

//�f�[�^�j��
CameraAccessor::~CameraAccessor()
{
	delete(m_camera);
}

//�C���X�^���X����
void CameraAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new CameraAccessor();
}

//�C���X�^���X�j��
void CameraAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}