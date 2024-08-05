#pragma once

class Camera;

class CameraAccessor
{
private:
	CameraAccessor();
	~CameraAccessor();
public:
	static void CreateInstance();
	static void DestroyInstance();
	static CameraAccessor* GetInstance() { return m_instance; }
	
	Camera* GetCamera() { return m_camera; }
private:
	static CameraAccessor* m_instance;
	Camera* m_camera;
};

