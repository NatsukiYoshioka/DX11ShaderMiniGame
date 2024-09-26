#pragma once
#include"GameObject.h"

class GameObject;
class OriginalEffect;

class DollHead:public GameObject
{
public:
	DollHead(const wchar_t* fileName, Vector3 pos);
	~DollHead();

	void InitializeTitle()override;
	void UpdateTitle()override;
	void DrawTitle()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void InitializeResult()override;
	void UpdateResult()override;
	void DrawResult()override;

	void DrawShadow();

private:
	shared_ptr<OriginalEffect> m_effect;
	Matrix m_world;
	float m_speed;
	float m_rotate;
	float m_rotationSpeed;

	const Vector3 m_firstPos;
	const Vector3 m_finalPos;
	const float m_firstSpeed;
	const float m_subSpeed;
	const float m_firstRotationSpeed;
	const float m_subRotationSpeed;
};