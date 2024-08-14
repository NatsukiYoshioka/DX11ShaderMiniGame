#include "pch.h"
#include"GameObject.h"
#include"DeviceAccessor.h"
#include "Camera.h"

//�J�����̏�����
Camera::Camera()
{
	m_modelHandle = nullptr;

	m_pos = Vector3(10.f, 5.f, -8.f);
	m_view = Matrix::CreateLookAt(m_pos, Vector3::Zero, Vector3::Up);

	auto size = DeviceAccessor::GetInstance()->GetScreenSize();
	m_projection = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, float(size.right) / float(size.bottom), 1.f, 5000.f);
}

//�f�[�^�j��
Camera::~Camera()
{

}

//�J�����̍X�V
void Camera::Update()
{

}

//�I�u�W�F�N�g�̕`��(�J�����̕`�揈���͖���)
void Camera::Draw()
{

}