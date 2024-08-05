#pragma once
using namespace DirectX;
using namespace SimpleMath;
using namespace std;

class GameObject
{
public:
	GameObject(){}
	~GameObject(){}
	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:
	unique_ptr<Model> m_modelHandle;
	Vector3 m_pos;
	float m_rotate;
};

