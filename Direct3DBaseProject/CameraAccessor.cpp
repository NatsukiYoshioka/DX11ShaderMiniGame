#include "pch.h"
#include"Camera.h"
#include "CameraAccessor.h"

CameraAccessor* CameraAccessor::m_instance = nullptr;

CameraAccessor::CameraAccessor()
{
	m_camera = new Camera();
}

CameraAccessor::~CameraAccessor()
{
	delete(m_camera);
}

void CameraAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new CameraAccessor();
}

void CameraAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}