#include "pch.h"
#include"Json.h"
#include"BaseScene.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"GameObject.h"
#include"DeviceAccessor.h"
#include"GiftBox.h"
#include"GiftBoxAccessor.h"
#include"Desk.h"
#include"DeskAccessor.h"
#include"Player.h"
#include"PlayerAccessor.h"
#include"Transition.h"
#include"TitleLogo.h"
#include"UIBase.h"
#include"UIAccessor.h"
#include "Camera.h"

//カメラの初期化
Camera::Camera():
	m_titlePosRatio(1.f),
	m_near(float(Json::GetInstance()->GetData()["CameraNearPlane"])),
	m_far(float(Json::GetInstance()->GetData()["CameraFarPlane"])),
	m_speed(float(Json::GetInstance()->GetData()["CameraPadSpeed"])),
	m_mouseSpeed(float(Json::GetInstance()->GetData()["CameraMouseSpeed"])),
	m_distance(float(Json::GetInstance()->GetData()["CameraDistance"])),
	m_minYaw(float(Json::GetInstance()->GetData()["CameraMinYaw"])),
	m_maxYaw(float(Json::GetInstance()->GetData()["CameraMaxYaw"])),
	m_titleInitializePos(Vector3(
		Json::GetInstance()->GetData()["CameraTitleEyePosition"].at(0),
		Json::GetInstance()->GetData()["CameraTitleEyePosition"].at(1),
		Json::GetInstance()->GetData()["CameraTitleEyePosition"].at(2)
	)),
	m_titleFinalPos(Vector3(
		Json::GetInstance()->GetData()["CameraTitleFinalEyePos"].at(0),
		Json::GetInstance()->GetData()["CameraTitleFinalEyePos"].at(1),
		Json::GetInstance()->GetData()["CameraTitleFinalEyePos"].at(2))
	),
	m_titleSpeed(float(Json::GetInstance()->GetData()["CameraTitleMoveSpeed"])),
	m_isFinishMoving(false),
	m_initializePitch(float(Json::GetInstance()->GetData()["CameraInitializePitch"])),
	m_initializeYaw(float(Json::GetInstance()->GetData()["CameraInitializeYaw"])),
	m_resultCameraHeight(float(Json::GetInstance()->GetData()["CameraResultHeight"])),
	m_moveWaitTime(0.f),
	m_maxMoveWaitTime(float(Json::GetInstance()->GetData()["CameraTitleLogoWaitTime"])),
	m_firstTargetPos(Vector3(
		Json::GetInstance()->GetData()["PlayerClearPosition"].at(0),
		Json::GetInstance()->GetData()["PlayerClearPosition"].at(1),
		Json::GetInstance()->GetData()["PlayerClearPosition"].at(2))),
	m_firstDistance(float(Json::GetInstance()->GetData()["CameraFirstDistance"])),
	m_maxFirstDistance(float(Json::GetInstance()->GetData()["CameraMaxFirstDistance"]))
{
	m_modelHandle = nullptr;

	auto size = DeviceAccessor::GetInstance()->GetScreenSize();
	m_projection = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, float(size.right) / float(size.bottom), m_near, m_far);
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
	m_pos = m_titleInitializePos;
	m_isFinishMoving = false;
	m_moveWaitTime = 0.f;
}

//タイトルシーンオブジェクトの更新
void Camera::UpdateTitle()
{
	auto title = dynamic_cast<TitleScene*>(SceneManager::GetInstance()->GetNowScene());
	//ゲームスタートしていたらUIをフェードアウトさせる
	if (title&&title->GetIsStartGame())
	{
		for (int i = 0;i < UIAccessor::GetInstance()->GetUIs().size();i++)
		{
			auto titleLogo = dynamic_cast<TitleLogo*>(UIAccessor::GetInstance()->GetUIs().at(i));
			if (titleLogo && titleLogo->GetAlpha() <= 0.f)
			{
				m_moveWaitTime += *DeviceAccessor::GetInstance()->GetElapsedTime();
				if (m_moveWaitTime >= m_maxMoveWaitTime)
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
				break;
			}
		}
	}
	//タイトルでの座標移動割合を計算
	m_titlePosRatio = 1.f - ((m_pos.LengthSquared() - m_titleInitializePos.LengthSquared()) / (m_titleFinalPos.LengthSquared() - m_titleInitializePos.LengthSquared()));
	//ビュー行列の設定
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
	auto json = Json::GetInstance();
	m_pitch = m_initializePitch;
	m_yaw = m_initializeYaw;
	m_distance = float(json->GetData()["CameraDistance"]);
	m_firstDistance = float(Json::GetInstance()->GetData()["CameraFirstDistance"]);
	m_isFinishMoving = false;
	m_moveWaitTime = 0.f;
}

//カメラの更新
void Camera::Update()
{
	if (!m_isFinishMoving)
	{
		m_moveWaitTime += *DeviceAccessor::GetInstance()->GetElapsedTime();
		if (m_moveWaitTime >= 1.f)
		{
			m_firstDistance += m_speed / 5.f;
		}
		if (m_firstDistance >= m_maxFirstDistance)
		{
			m_firstDistance = m_maxFirstDistance;
			for (int i = 0;i < UIAccessor::GetInstance()->GetUIs().size();i++)
			{
				auto transition = dynamic_cast<Transition*>(UIAccessor::GetInstance()->GetUIs().at(i));
				if (transition)
				{
					//トランジションをスタートさせる
					transition->SetTransitionStart(true);
					if (transition->GetIsFinishFadeout())
					{
						m_isFinishMoving = true;
						transition->SetTransitionStart(false);
					}
					break;
				}
			}
		}

		float radianX = m_pitch * XM_PI / 180;
		float radianY = m_yaw * XM_PI / 180;

		float z = m_firstTargetPos.z + cos(radianY) * cos(radianX) * m_firstDistance;
		float x = m_firstTargetPos.x + cos(radianY) * sin(radianX) * m_firstDistance;
		float y = m_firstTargetPos.y + sin(radianY) * m_firstDistance;
		m_pos = Vector3(x, y, z);
		m_view = Matrix::CreateLookAt(m_pos, m_firstTargetPos, Vector3::Up);

		return;
	}
	auto pad = DeviceAccessor::GetInstance()->GetGamePad()->GetState(0);
	auto mouseState = DeviceAccessor::GetInstance()->GetMouse()->GetState();

	//視点移動処理
	if (pad.thumbSticks.rightX != 0 || pad.thumbSticks.rightY != 0 || mouseState.x != 0 || mouseState.y != 0)
	{
		float x = pad.thumbSticks.rightX;
		float y = pad.thumbSticks.rightY;
		float pitchSpeed = m_speed;
		float yawSpeed = m_speed;
		if (mouseState.x != 0 || mouseState.y != 0)
		{
			x = (float)mouseState.x;
			y = (float)-mouseState.y;
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
	
	//カメラの移動限界処理
	if (m_yaw < m_minYaw)
	{
		m_yaw = m_minYaw;
	}
	if (m_yaw > m_maxYaw)
	{
		m_yaw = m_maxYaw;
	}

	//カメラの注視点をプレイヤーにする
	auto playerPos = PlayerAccessor::GetInstance()->GetPlayer()->GetPos();
	playerPos.y += 2.5f;
	float radianX = m_pitch * XM_PI / 180;
	float radianY = m_yaw * XM_PI / 180;

	float z = playerPos.z + cos(radianY) * cos(radianX) * m_distance;
	float x = playerPos.x + cos(radianY) * sin(radianX) * m_distance;
	float y = playerPos.y + sin(radianY) * m_distance;
	m_pos = Vector3(x, y, z);

	//ビュー行列の設定
	m_view = Matrix::CreateLookAt(m_pos, playerPos, Vector3::Up);
}

//オブジェクトの描画(カメラの描画処理は無し)
void Camera::Draw()
{

}

//リザルトシーンオブジェクトの初期化
void Camera::InitializeResult()
{
	auto json = Json::GetInstance();
	//クリア時の初期化
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())
	{
		m_pitch = float(json->GetData()["CameraInitializeClearPitch"]);
		m_yaw = float(json->GetData()["CameraInitializeClearYaw"]);
		m_distance = float(json->GetData()["CameraClearDistance"]);
	}
	//ゲームオーバー時の初期化
	else
	{
		m_pos = Vector3(json->GetData()["CameraGameOverPos"].at(0),
			json->GetData()["CameraGameOverPos"].at(1),
			json->GetData()["CameraGameOverPos"].at(2));
		m_view= Matrix::CreateLookAt(m_pos, DeskAccessor::GetInstance()->GetDesk()->GetPos(), Vector3::Up);
	}
}

//リザルトシーンオブジェクトの更新
void Camera::UpdateResult()
{
	//クリア時のカメラ更新
	if (PlayerAccessor::GetInstance()->GetPlayer()->GetIsClear())
	{
		float radianX = m_pitch * XM_PI / 180;
		float radianY = m_yaw * XM_PI / 180;

		auto playerPos = PlayerAccessor::GetInstance()->GetPlayer()->GetPos();
		float z = playerPos.z + cos(radianX) * m_distance;
		float x = playerPos.x + sin(radianX) * m_distance;
		float y = playerPos.y + tan(radianY) * m_distance;
		m_pos = Vector3(x, y, z);

		auto targetPos = playerPos;
		targetPos.y += m_resultCameraHeight;
		m_view = Matrix::CreateLookAt(m_pos, targetPos, Vector3::Up);
	}
}

//リザルトシーンオブジェクトの描画(カメラの描画処理は無し)
void Camera::DrawResult()
{

}