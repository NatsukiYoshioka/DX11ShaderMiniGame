#pragma once
#include"UIBase.h"

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

class UIBase;

class Transition :public UIBase
{
public:
	/// <summary>
	/// �g�����W�V�����̏�����
	/// </summary>
	Transition();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~Transition();

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

	/// <summary>
	/// �g�����W�V�����̃t�F�[�h�A�E�g���I��������ǂ����擾
	/// </summary>
	/// <returns></returns>
	bool GetIsFinishFadeout()const { return m_isFinishFadeout; }

	/// <summary>
	/// �g�����W�V�����̃t�F�[�h�C�����I��������ǂ����擾
	/// </summary>
	/// <returns></returns>
	bool GetIsFinishFadein()const { return m_isFinishFadein; }

private:
	ComPtr<ID3D11ShaderResourceView> m_texture;		//�e�N�X�`��
	ComPtr<ID3D11Resource> m_textureResource;		//�e�N�X�`�����

	Vector2 m_origin;		//�e�N�X�`�����S���W
	float m_alpha;			//�e�N�X�`���̃A���t�@�l

	bool m_isFinishFadeout;	//�t�F�[�h�A�E�g���I��������ǂ���
	bool m_isFinishFadein;	//�t�F�[�h�C�����I��������ǂ���

	const float m_scale;		//�e�N�X�`���̃X�P�[��
	const float m_layerDepth;	//�e�N�X�`���̃��C���[�[�x
	const float m_alphaAdd;		//�A���t�@�l�̑�����
};

