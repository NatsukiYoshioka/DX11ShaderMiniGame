#pragma once

using namespace Microsoft::WRL;

class UIBase;

/// <summary>
/// �����蔻��UI
/// </summary>
class FoundUI:public UIBase
{
public:
	/// <summary>
	/// UI������
	/// </summary>
	FoundUI();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~FoundUI();

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
	/// ���S�Ɍ����������ǂ����擾
	/// </summary>
	/// <returns></returns>
	bool GetIsFound()const { return m_isFound; }

private:
	ComPtr<ID3D11ShaderResourceView> m_baseTexture;		//�f�t�H���g�̃e�N�X�`��
	ComPtr<ID3D11ShaderResourceView> m_foundTexture;	//�����蔻�茋�ʕ\���e�N�X�`��

	ComPtr<ID3D11Resource> m_baseTextureResource;		//�f�t�H���g�̃e�N�X�`�����
	ComPtr<ID3D11Resource> m_foundTextureResource;		//�����蔻�茋�ʕ\���e�N�X�`�����

	Vector2 m_baseTexturePos;		//�f�t�H���g�e�N�X�`�����W
	Vector2 m_foundTexturePos;		//�����蔻��e�N�X�`�����W

	RECT m_rectangle;		//�e�N�X�`���̕\���͈�

	float m_baseTextureRotation;	//�f�t�H���g�e�N�X�`����]��
	float m_foundTextureRotation;	//�����蔻��e�N�X�`����]��

	Vector2 m_baseTextureOrigin;	//�f�t�H���g�e�N�X�`�����S���W
	Vector2 m_foundTextureOrigin;	//�����蔻��e�N�X�`�����S���W

	Vector2 m_baseTextureScale;		//�f�t�H���g�e�N�X�`���X�P�[��
	Vector2 m_foundTextureScale;	//�����蔻��e�N�X�`���X�P�[��

	float m_baseTextureDepth;		//�f�t�H���g�e�N�X�`���[�x
	float m_foundTextureDepth;		//�����蔻��e�N�X�`���[�x
		
	float m_foundTime;				//�����葱���Ă��鎞��
	const float m_maxFoundTime;		//�����܂ł̎���

	XMVECTOR m_color;		//�e�N�X�`���̃J���[
	float m_textureHeight;	//�e�N�X�`���̍���

	bool m_isFound;			//���S�Ɍ����������ǂ���
};

