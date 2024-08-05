#pragma once
#include"GameObject.h"
using namespace DirectX::SimpleMath;

class GameObject;

class Camera:public GameObject
{
public:
	Camera();
	~Camera();

	void Update()override;
	void Draw()override;

	Matrix GetView()const { return m_view; }
	Matrix GetProjection()const { return m_projection; }

private:
	Matrix m_view;
	Matrix m_projection;
};

