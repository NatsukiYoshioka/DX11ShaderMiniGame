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

	/// <summary>
	/// �e�̕`��
	/// </summary>
	void DrawShadow();

	/// <summary>
	/// �G���_�ł̕`��
	/// </summary>
	void DrawHitCheck();

	/// <summary>
	/// ���_���W�̎擾
	/// </summary>
	/// <returns></returns>
	vector<XMFLOAT3> GetVertices() { return m_vertices; }

	/// <summary>
	/// ���_���W��SRV�擾
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D11ShaderResourceView> GetVertexBufferSRV() { return m_vertexBufferSRV; }

private:
	shared_ptr<OriginalEffect> m_effect;		//���f���`��p�G�t�F�N�g�N���X

	float m_rotate;		//���f����Y����]��
	Matrix m_world;		//���f���̃��[���h���W�s��

	vector<XMFLOAT3> m_vertices;		//���f���̒��_���W
	ComPtr<ID3D11ShaderResourceView> m_vertexBufferSRV;		//���_�o�b�t�@SRV
	ComPtr<ID3D11UnorderedAccessView> m_outputBufferUAV;	//�o�̓o�b�t�@UAV
};

