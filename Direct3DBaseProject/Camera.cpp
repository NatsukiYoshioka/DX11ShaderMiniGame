#include "pch.h"
#include"GameObject.h"
#include"DeviceAccessor.h"
#include "Camera.h"

//カメラの初期化
Camera::Camera()
{
	m_modelHandle = nullptr;

	m_pos = Vector3(10.f, 5.f, -8.f);
	m_view = Matrix::CreateLookAt(m_pos, Vector3::Zero, Vector3::Up);

	auto size = DeviceAccessor::GetInstance()->GetScreenSize();
	m_projection = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, float(size.right) / float(size.bottom), 1.f, 5000.f);
}

//データ破棄
Camera::~Camera()
{

}

//カメラの更新
void Camera::Update()
{

}

//オブジェクトの描画(カメラの描画処理は無し)
void Camera::Draw()
{

}