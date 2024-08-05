#include "pch.h"
#include"GameObject.h"
#include"DeviceAccessor.h"
#include "Camera.h"

Camera::Camera()
{
	m_modelHandle = nullptr;
	m_rotate = NULL;

	m_pos = Vector3(10.f, 5.f, -8.f);
	m_view = Matrix::CreateLookAt(m_pos, Vector3::Zero, Vector3::Up);

	auto size = DeviceAccessor::GetInstance()->GetScreenSize();
	m_projection = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, float(size.right) / float(size.bottom), 1.f, 50.f);
}

Camera::~Camera()
{

}

void Camera::Update()
{

}

void Camera::Draw()
{

}