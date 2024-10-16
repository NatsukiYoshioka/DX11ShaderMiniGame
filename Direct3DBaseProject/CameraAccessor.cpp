#include "pch.h"
#include"Camera.h"
#include "CameraAccessor.h"

CameraAccessor* CameraAccessor::m_instance = nullptr;

//アクセサの初期化
CameraAccessor::CameraAccessor()
{
	m_camera = new Camera();
}

//データ破棄
CameraAccessor::~CameraAccessor()
{
	delete(m_camera);
}

//インスタンス生成
void CameraAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new CameraAccessor();
}

//インスタンス破棄
void CameraAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}