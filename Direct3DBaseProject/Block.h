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
	Block(Model* modelHandle, vector<Vector3> pos, vector<float> rotate, vector<int> textureID);

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~Block();

	/// <summary>
	/// �^�C�g���V�[���I�u�W�F�N�g�̏�����
	/// </summary>
	void InitializeTitle()override;

	/// <summary>
	/// �^�C�g���V�[���I�u�W�F�N�g�̍X�V
	/// </summary>
	void UpdateTitle()override;

	/// <summary>
	/// �^�C�g���V�[���I�u�W�F�N�g�̕`��
	/// </summary>
	void DrawTitle()override;

	/// <summary>
	/// �I�u�W�F�N�g�̏�����
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// �I�u�W�F�N�g�̍X�V
	/// </summary>
	void Update()override;

	/// <summary>
	/// �I�u�W�F�N�g�̕`��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// ���U���g�V�[���I�u�W�F�N�g�̏�����
	/// </summary>
	void InitializeResult()override;

	/// <summary>
	/// ���U���g�V�[���I�u�W�F�N�g�̍X�V
	/// </summary>
	void UpdateResult()override;

	/// <summary>
	/// ���U���g�V�[���I�u�W�F�N�g�̕`��
	/// </summary>
	void DrawResult()override;

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
	Model* m_model;								//���f����񂪊i�[���ꂽ�N���X�|�C���^

	/// <summary>
	/// �`�掞�̃C���X�^���V���O�ɕK�v�ȕϐ��̍\����
	/// </summary>
	struct InstanceData
	{
		XMMATRIX world;			//�I�u�W�F�N�g�̃��[���h���W
		int textureID;			//�I�u�W�F�N�g�̃e�N�X�`��ID
	};
	vector<InstanceData> m_instances;			//�C���X�^���V���O�\���̂̃R���e�i
	ComPtr<ID3D11Buffer> m_instanceBuffer;		//�C���X�^���V���O�\���̃o�b�t�@
	shared_ptr<OriginalEffect> m_effect;		//���f���`��p�G�t�F�N�g�N���X
	ComPtr<ID3D11InputLayout> m_inputLayout;

	float m_rotate;		//���f����Y����]��
	Matrix m_world;		//���f���̃��[���h���W�s��

	vector<XMFLOAT3> m_vertices;		//���f���̒��_���W
	ComPtr<ID3D11ShaderResourceView> m_vertexBufferSRV;		//���_�o�b�t�@SRV
	ComPtr<ID3D11UnorderedAccessView> m_outputBufferUAV;	//�o�̓o�b�t�@UAV
};

