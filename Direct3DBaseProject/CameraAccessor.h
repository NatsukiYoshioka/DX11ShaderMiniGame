#pragma once

class Camera;

/// <summary>
/// カメラのアクセサ
/// </summary>
class CameraAccessor
{
private:
	/// <summary>
	/// カメラの生成
	/// </summary>
	CameraAccessor();

	/// <summary>
	/// カメラの破棄
	/// </summary>
	~CameraAccessor();
public:
	/// <summary>
	/// インスタンス生成
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// インスタンス破棄
	/// </summary>
	static void DestroyInstance();

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>アクセサのインスタンス</returns>
	static CameraAccessor* GetInstance() { return m_instance; }
	
	/// <summary>
	/// カメラの取得
	/// </summary>
	/// <returns>カメラのインスタンス</returns>
	Camera* GetCamera() { return m_camera; }
private:
	static CameraAccessor* m_instance;		//アクセサのインスタンス
	Camera* m_camera;						//カメラのインスタンス
};

