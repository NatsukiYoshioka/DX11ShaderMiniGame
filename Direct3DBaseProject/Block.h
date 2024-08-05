#pragma once
#include"GameObject.h"
using namespace DirectX;
using namespace SimpleMath;

class GameObject;

class Block:public GameObject
{
public:
	Block(const wchar_t* fileName, Vector3 pos, float rotate);
	~Block();

	void Update()override;
	void Draw()override;

private:
	Matrix m_world;
};

