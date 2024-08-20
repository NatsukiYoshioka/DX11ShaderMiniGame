#pragma once
#include"GameObject.h"
using namespace DirectX;
using namespace SimpleMath;
using namespace Microsoft::WRL;
using namespace std;

class GameObject;
class OriginalEffect;

/// <summary>
/// �ςݖ؃u���b�N�I�u�W�F�N�g
/// </summary>
class Block:public GameObject
{
public:
	/// <summary>
	/// �ςݖ؃u���b�N�̏�����
	/// </summary>
	/// <param name="fileName">3D���f���̃t�@�C����</param>
	/// <param name="pos">�I�u�W�F�N�g�̏������W</param>
	/// <param name="rotate">�I�u�W�F�N�g��Y����]��(XM_PI / rotate)</param>
	Block(const wchar_t* fileName, Vector3 pos, float rotate);

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~Block();

	/// <summary>
	/// �I�u�W�F�N�g�̍X�V
	/// </summary>
	void Update()override;

	/// <summary>
	/// �I�u�W�F�N�g�̕`��
	/// </summary>
	void Draw()override;

private:
	shared_ptr<OriginalEffect> m_effect;		//���f���`��p�G�t�F�N�g�N���X

	float m_rotate;		//���f����Y����]��
	Matrix m_world;		//���f���̃��[���h���W�s��
};

