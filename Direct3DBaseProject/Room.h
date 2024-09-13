#pragma once
#include"GameObject.h"

using namespace std;
using namespace Microsoft::WRL;

class GameObject;
class OriginalEffect;

class Room:public GameObject
{
public:
	Room(const wchar_t* fileName);
	~Room();

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
	vector<ComPtr<ID3D11ShaderResourceView>> texture;
	vector<shared_ptr<OriginalEffect>> m_effect;

	Matrix m_world;
	float m_rotate;
};

