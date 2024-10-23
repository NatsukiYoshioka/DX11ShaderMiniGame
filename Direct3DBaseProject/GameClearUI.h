#pragma once

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

class UIBase;

/// <summary>
/// �Q�[���N���AUI�Ǘ��N���X
/// </summary>
class GameClearUI:public UIBase
{
public:
	/// <summary>
	/// �N���A����UI������
	/// </summary>
	GameClearUI();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~GameClearUI();

	/// <summary>
	/// �^�C�g���V�[��UI������
	/// </summary>
	void InitializeTitle()override;

	/// <summary>
	/// �^�C�g���V�[��UI�X�V
	/// </summary>
	void UpdateTitle()override;

	/// <summary>
	/// �^�C�g���V�[��UI�`��
	/// </summary>
	void DrawTitle()override;

	/// <summary>
	/// UI������
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// UI�X�V
	/// </summary>
	void Update()override;

	/// <summary>
	/// UI�`��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// ���U���g�V�[��UI������
	/// </summary>
	void InitializeResult()override;

	/// <summary>
	/// ���U���g�V�[��UI�X�V
	/// </summary>
	void UpdateResult()override;

	/// <summary>
	/// ���U���g�V�[��UI�`��
	/// </summary>
	void DrawResult()override;

private:
	ComPtr<ID3D11ShaderResourceView> m_texture;		//�e�N�X�`��
	ComPtr<ID3D11Resource> m_textureResource;		//�e�N�X�`�����

	Vector2 m_pos;		//�e�N�X�`���̍��W
	Vector2 m_origin;	//�e�N�X�`���̒��S���W

	const float m_scale;		//�e�N�X�`���̃X�P�[��
	const float m_layerDepth;	//�e�N�X�`���̃��C���[�[�x
};

