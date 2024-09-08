#include "pch.h"
#include"Json.h"
#include"GameObject.h"
#include"DeviceAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include "Camera.h"

//カメラの初期化
Camera::Camera():
	m_speed(float(Json::GetInstance()->GetData()["CameraPadSpeed"])),
	m_mouseSpeed(float(Json::GetInstance()->GetData()["CameraMouseSpeed"])),
	m_distance(float(Json::GetInstance()->GetData()["CameraDistance"])),
	m_minYaw(float(Json::GetInstance()->GetData()["CameraMinYaw"])),
	m_maxYaw(float(Json::GetInstance()->GetData()["CameraMaxYaw"])),
	m_pitch(20),
	m_yaw(40)
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
	auto mouse = DeviceAccessor::GetInstance()->GetMouse();
	mouse->SetMode(Mouse::MODE_RELATIVE);
	auto mouseState = mouse->GetState();

	if (pad.IsConnected())
	{
		//視点移動処理
		if (pad.thumbSticks.rightX != 0 || pad.thumbSticks.rightY != 0 || mouseState.x != 0 || mouseState.y != 0)
		{
			float x = pad.thumbSticks.rightX;
			float y = pad.thumbSticks.rightY;
			float pitchSpeed = m_speed;
			float yawSpeed = m_speed;
			if (mouseState.x != 0|| mouseState.y != 0)
			{
				x = mouseState.x;
				y = -mouseState.y;
				pitchSpeed = m_speed * (x / m_mouseSpeed);
				if (pitchSpeed < 0)pitchSpeed *= -1;
				yawSpeed = m_speed * (y / m_mouseSpeed);
				if (yawSpeed < 0)yawSpeed *= -1;
			}
			auto moveDirection = Vector2(x, y);
			moveDirection.Normalize();
			m_pitch += -moveDirection.x * pitchSpeed;
			m_yaw += -moveDirection.y * yawSpeed;
		}
	}
	
	if (m_yaw < m_minYaw)
	{
		m_yaw = m_minYaw;
	}
	if (m_yaw > m_maxYaw)
	{
		m_yaw = m_maxYaw;
	}

	auto playerPos = PlayerAccessor::GetInstance()->GetPlayer()->GetPos();
	float radianX = m_pitch * XM_PI / 180;
	float radianY = m_yaw * XM_PI / 180;

	float z = playerPos.z + cos(radianX) * m_distance;
	float x = playerPos.x + sin(radianX) * m_distance;
	float y = playerPos.y+0.5f + tan(radianY) * m_distance;
	m_pos = Vector3(x, y, z);

	m_view = Matrix::CreateLookAt(m_pos, playerPos, Vector3::Up);
}

//オブジェクトの描画(カメラの描画処理は無し)
void Camera::Draw()
{

}