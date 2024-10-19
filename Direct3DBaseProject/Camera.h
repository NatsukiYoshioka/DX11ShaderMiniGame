#pragma once
#include"GameObject.h"
using namespace DirectX::SimpleMath;

class GameObject;

/// <summary>
/// カメラオブジェクト
/// </summary>
class Camera:public GameObject
{
public:
	/// <summary>
	/// カメラの初期化
	/// </summary>
	Camera();

	/// <summary>
	/// データ破棄
	/// </summary>
	~Camera();

	void InitializeTitle()override;

	/// <summary>
	/// タイトルシーンオブジェクトの更新
	/// </summary>
	void UpdateTitle()override;

	/// <summary>
	/// タイトルシーンオブジェクトの描画(描画なし)
	/// </summary>
	void DrawTitle()override;

	void Initialize()override;

	/// <summary>
	/// オブジェクトの更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// オブジェクトの描画(カメラなので描画処理は無し)
	/// </summary>
	void Draw()override;

	void InitializeResult()override;

	/// <summary>
	/// リザルトシーンオブジェクトの更新
	/// </summary>
	void UpdateResult()override;

	/// <summary>
	/// リザルトシーンオブジェクトの描画(描画なし)
	/// </summary>
	void DrawResult()override;

	void DrawShadow()override{}

	/// <summary>
	/// カメラビュー行列の取得
	/// </summary>
	/// <returns>ビュー行列</returns>
	Matrix GetView()const { return m_view; }

	/// <summary>
	/// カメラプロジェクション行列の取得
	/// </summary>
	/// <returns>プロジェクション行列</returns>
	Matrix GetProjection()const { return m_projection; }

	/// <summary>
	/// カメラのピッチ取得
	/// </summary>
	/// <returns>カメラのピッチ</returns>
	float GetPitch()const  { return m_pitch; }

	/// <summary>
	/// タイトルでのカメラの移動割合
	/// </summary>
	/// <returns></returns>
	float GetTitlePosRatio()const { return m_titlePosRatio; }

	/// <summary>
	/// タイトルでのカメラ移動が終了したか取得
	/// </summary>
	/// <returns></returns>
	bool GetIsFinishMoving()const { return m_isFinishMoving; }

private:
	Matrix m_view;			//カメラのビュー行列
	Matrix m_projection;	//カメラのプロジェクション行列
	float m_distance;		//カメラとプレイヤーとの距離
	float m_pitch;			//カメラの縦軸回転量
	float m_yaw;			//カメラの横軸回転量

	float m_titlePosRatio;	//タイトルの座標移動割合

	const float m_speed;		//カメラのデフォルトスピード
	const float m_mouseSpeed;	//カメラのマウススピード

	const float m_minYaw;		//カメラの最小ヨー
	const float m_maxYaw;		//カメラの最大ヨー

	float m_moveWaitTime;				//タイトルでのカメラ移動までの待機時間
	const float m_maxMoveWaitTime;		//タイトルでのカメラ移動までの最大待機時間
	bool m_isFinishMoving;				//タイトルのカメラ移動が終了したかどうか
	const Vector3 m_titleInitializePos;	//タイトルでの初期座標
	const Vector3 m_titleFinalPos;		//タイトルでの最終座標
	const float m_titleSpeed;			//タイトルでの座標移動スピード

	const float m_initializePitch;		//カメラの初期ピッチ
	const float m_initializeYaw;		//カメラの初期ヨー
	const float m_resultCameraHeight;	//リザルトでのカメラの高さ
};

