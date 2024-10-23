#pragma once
#include"UIBase.h"
using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

class UIBase;

/// <summary>
/// A�{�^���Ǘ��N���X
/// </summary>
class ButtonA:public UIBase
{
public:
	/// <summary>
	/// A�{�^��UI�̏�����
	/// </summary>
	ButtonA();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~ButtonA();

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

	/// <summary>
	/// �e�N�X�`���̃X�P�[���̍X�V
	/// </summary>
	void UpdateScale();

private:
	ComPtr<ID3D11ShaderResourceView> m_texture;		//�e�N�X�`��
	ComPtr<ID3D11Resource> m_textureResource;		//�e�N�X�`�����

	Vector2 m_pos;		//�e�N�X�`���̍��W
	Vector2 m_origin;	//�e�N�X�`���̒��S���W
	float m_alpha;		//�e�N�X�`���̃A���t�@�l
	float m_scale;		//�e�N�X�`���̃X�P�[��
	float m_scaleAdd;	//�e�N�X�`���̃X�P�[���g�k��

	const float m_layerDepth;	//�e�N�X�`���̃��C���[�[�x
	const float m_minScale;		//�e�N�X�`���̍ŏ��X�P�[��
	const float m_maxScale;		//�e�N�X�`���̍ő�X�P�[��
};

