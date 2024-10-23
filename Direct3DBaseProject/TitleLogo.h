#pragma once
#include"UIBase.h"
using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

class UIBase;

/// <summary>
/// �^�C�g�����S�Ǘ��N���X
/// </summary>
class TitleLogo:public UIBase
{
public:
	/// <summary>
	/// �^�C�g�����S�̏�����
	/// </summary>
	TitleLogo();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~TitleLogo();

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
	/// �e�N�X�`���̃A���t�@�l�擾
	/// </summary>
	/// <returns></returns>
	float GetAlpha()const { return m_alpha; }

private:
	ComPtr<ID3D11ShaderResourceView> m_texture;		//�e�N�X�`��
	ComPtr<ID3D11Resource> m_textureResource;		//�e�N�X�`�����

	Vector2 m_pos;		//�e�N�X�`�����W
	Vector2 m_origin;	//�e�N�X�`�����S���W
	float m_alpha;		//�e�N�X�`���A���t�@�l
	float m_renderTime;	//�e�N�X�`����`�悵���o�ߎ���

	const float m_scale;		//�e�N�X�`���̃X�P�[��
	const float m_layerDepth;	//�e�N�X�`���̃��C���[�[�x
	const float m_fadeoutTime;	//�e�N�X�`�����t�F�[�h�A�E�g����܂ł̎���
};

