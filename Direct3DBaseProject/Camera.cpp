#include "pch.h"
#include"Json.h"
#include"BaseScene.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"GameObject.h"
#include"DeviceAccessor.h"
#include"GiftBox.h"
#include"GiftBoxAccessor.h"
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
	m_titleFinalPos(Vector3(
		Json::GetInstance()->GetData()["CameraTitleFinalEyePos"].at(0),
		Json::GetInstance()->GetData()["CameraTitleFinalEyePos"].at(1),
		Json::GetInstance()->GetData()["CameraTitleFinalEyePos"].at(2))
	),
	m_titleSpeed(float(Json::GetInstance()->GetData()["CameraTitleMoveSpeed"])),
	m_isFinishMoving(false),
	m_initializePitch(float(Json::GetInstance()->GetData()["CameraInitializePitch"])),
	m_initializeYaw(float(Json::GetInstance()->GetData()["CameraInitializeYaw"]))
{
	m_modelHandle = nullptr;

	auto size = DeviceAccessor::GetInstance()->GetScreenSize();
	m_projection = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, float(size.right) / float(size.bottom), 1.f, 5000.f);
	m_pitch = m_initializePitch;
	m_yaw = m_initializeYaw;
}

//データ破棄
Camera::~Camera()
{

}

//タイトルシーンオブジェクトの初期化
void Camera::InitializeTitle()
{
	m_pos = Vector3(Vector3(Json::GetInstance()->GetData()["CameraTitleEyePosition"].at(0),
		Json::GetInstance()->GetData()["CameraTitleEyePosition"].at(1),
		Json::GetInstance()->GetData()["CameraTitleEyePosition"].at(2)));
	m_isFinishMoving = false;
}

//タイトルシーンオブジェクトの更新
void Camera::UpdateTitle()
{
	auto title = dynamic_cast<TitleScene*>(SceneManager::GetInstance()->GetNowScene());
	if (title&&title->GetIsStartGame())
	{
		auto direction = m_titleFinalPos - m_pos;
		direction.Normalize();
		m_pos += direction * m_titleSpeed;
		if (m_pos.LengthSquared() >= m_titleFinalPos.LengthSquared())
		{
			m_pos = m_titleFinalPos;
			m_isFinishMoving = true;
		}
	}
	m_view = Matrix::CreateLookAt(
		m_pos,
		PlayerAccessor::GetInstance()->GetPlayer()->GetPos(), Vector3::Up);
}

//タイトルシーンオブジェクトの描画(カメラの描画処理は無し)
void Camera::DrawTitle()
{

}

//カメラの初期化
void Camera::Initialize()
{
	m_pitch = m_initializePitch;
	m_yaw = m_initializeYaw;
}

//カメラの更新
void Camera::Update()
{
	auto pad = DeviceAccessor::GetInstance()->GetGamePad()->GetState(0);
	auto mouse = DeviceAccessor::GetInstance()->GetMouse();
	//mouse->SetMode(Mouse::MODE_RELATIVE);
	auto mouseState = mouse->GetState();

	//視点移動処理
	if (pad.thumbSticks.rightX != 0 || pad.thumbSticks.rightY != 0 || mouseState.x != 0 || mouseState.y != 0)
	{
		float x = pad.thumbSticks.rightX;
		float y = pad.thumbSticks.rightY;
		float pitchSpeed = m_speed;
		float yawSpeed = m_speed;
		if (mouseState.x != 0 || mouseState.y != 0)
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
	float y = playerPos.y + tan(radianY) * m_distance;
	m_pos = Vector3(x, y, z);

	m_view = Matrix::CreateLookAt(m_pos, playerPos, Vector3::Up);
}

//オブジェクトの描画(カメラの描画処理は無し)
void Camera::Draw()
{

}

//リザルトシーンオブジェクトの初期化
void Camera::InitializeResult()
{

}

//リザルトシーンオブジェクトの更新
void Camera::UpdateResult()
{

}

//リザルトシーンオブジェクトの描画(カメラの描画処理は無し)
void Camera::DrawResult()
{

}