#include "pch.h"
#include"Json.h"
#include"GameObject.h"
#include"DeviceAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include "Camera.h"

//カメラの初期化
Camera::Camera():
	m_speed(float(Json::GetInstance()->GetData()["CameraSpeed"])),
	m_distance(float(Json::GetInstance()->GetData()["CameraDistance"])),
	m_pitch(0),
	m_yaw(0)
{
	m_modelHandle = nullptr;

	auto size = DeviceAccessor::GetInstance()->GetScreenSize();
	m_projection = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, float(size.right) / float(size.bottom), 1.f, 500.f);
}

//データ破棄
Camera::~Camera()
{

}

//カメラの更新
void Camera::Update()
{
	auto pad = DeviceAccessor::GetInstance()->GetGamePad()->GetState(0);

	if (pad.IsConnected())
	{
		if (pad.thumbSticks.rightX != 0 || pad.thumbSticks.rightY != 0)
		{
			auto moveDirection = Vector2(pad.thumbSticks.rightX, pad.thumbSticks.rightY);
			moveDirection.Normalize();
			m_pitch += -moveDirection.x * m_speed;
			m_yaw += -moveDirection.y * m_speed;
		}
	}
	
	if (m_yaw < 10)
	{
		m_yaw = 10;
	}
	if (m_yaw > 45)
	{
		m_yaw = 45;
	}

	auto playerPos = PlayerAccessor::GetInstance()->GetPlayer()->GetPos();
	float radianX = m_pitch * XM_PI / 180;
	float radianY = m_yaw * XM_PI / 180;

	float z = playerPos.z + cos(radianX) * m_distance;
	float x = playerPos.x + sin(radianX) * m_distance;
	float y = playerPos.y + tan(radianY) * m_distance;
	m_pos = Vector3(x, y, z);

	m_view = Matrix::CreateLookAt(m_pos, playerPos, Vector3::Up);
}

//オブジェクトの描画(カメラの描画処理は無し)
void Camera::Draw()
{

}