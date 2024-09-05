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

	/// <summary>
	/// オブジェクトの更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// オブジェクトの描画(カメラなので描画処理は無し)
	/// </summary>
	void Draw()override;

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

private:
	Matrix m_view;			//カメラのビュー行列
	Matrix m_projection;	//カメラのプロジェクション行列
	float m_distance;		//カメラとプレイヤーとの距離
	float m_pitch;			//カメラの縦軸回転量
	float m_yaw;			//カメラの横軸回転量

	const float m_speed;
	const float m_mouseSpeed;
};

