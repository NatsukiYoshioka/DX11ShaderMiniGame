#pragma once
#include"GameObject.h"

class GameObject;
class OriginalEffect;

class GiftBox:public GameObject
{
public:
	GiftBox(const wchar_t* fileName, Vector3 pos, float rotate);
	~GiftBox();

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
	shared_ptr<OriginalEffect> m_effect;		//���f���`��p�G�t�F�N�g�N���X

	float m_rotate;
	Matrix m_world;		//���f���̃��[���h�s��
};

