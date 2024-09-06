#pragma once

using namespace DirectX;
using namespace SimpleMath;
using namespace Microsoft::WRL;
using namespace std;

/// <summary>
/// ���f���`��p����G�t�F�N�g�N���X
/// </summary>
class OriginalEffect :public IEffect, public IEffectMatrices, public IEffectSkinning
{
public:
	/// <summary>
	/// �s�N�Z���V�F�[�_�[�̃^�C�v
	/// </summary>
	enum class PixelType
	{
		Object,
		Character,
		Shadow,
		Red,
		Blue
	};

	/// <summary>
	/// �s�N�Z���V�F�[�_�[�̃^�C�v�X�V
	/// </summary>
	/// <param name="value"></param>
	void UpdateType(PixelType value) { m_type = value; }

	/// <summary>
	/// �G�t�F�N�g�̏�����
	/// </summary>
	/// <param name="device">�`��f�o�C�X</param>
	explicit OriginalEffect(ID3D11Device* device, PixelType type, bool isSkinning = false);

	/// <summary>
	/// �G�t�F�N�g�̓K�p����
	/// </summary>
	/// <param name="context">�`��R���e�L�X�g</param>
	virtual void Apply(ID3D11DeviceContext* context);

	/// <summary>
	/// ���_�V�F�[�_�[�̃o�C�g�R�[�h�擾
	/// </summary>
	/// <param name="pShaderBytecode">�o�C�g�R�[�h</param>
	/// <param name="pBytecodeLength">�o�C�g�R�[�h�̃T�C�Y</param>
	virtual void GetVertexShaderBytecode(void const** pShaderBytecode, size_t* pBytecodeLength)override;

	/// <summary>
	/// �e�N�X�`���̐ݒ�
	/// </summary>
	/// <param name="value">�e�N�X�`����SRV</param>
	void SetTexture(ID3D11ShaderResourceView* value);

	/// <summary>
	/// �@���}�b�v�̐ݒ�
	/// </summary>
	/// <param name="value">�@���}�b�v��SRV</param>
	void SetNormal(ID3D11ShaderResourceView* value);

	/// <summary>
	/// AO�}�b�v�̐ݒ�
	/// </summary>
	/// <param name="value">AO�}�b�v��SRV</param>
	void SetAO(ID3D11ShaderResourceView* value);

	/// <summary>
	/// ���[���h�s��̐ݒ�
	/// </summary>
	/// <param name="world">���[���h�s��</param>
	void XM_CALLCONV SetWorld(FXMMATRIX world)override;

	/// <summary>
	/// �r���[�s��̐ݒ�
	/// </summary>
	/// <param name="view">�r���[�s��</param>
	void XM_CALLCONV SetView(FXMMATRIX view)override;

	/// <summary>
	/// �v���W�F�N�V�����s��̐ݒ�
	/// </summary>
	/// <param name="projection">�v���W�F�N�V�����s��</param>
	/// <returns></returns>
	void XM_CALLCONV SetProjection(FXMMATRIX projection)override;

	/// <summary>
	/// ���[���h,�r���[,���W�F�N�V�����s��̐ݒ�
	/// </summary>
	/// <param name="world">���[���h�s��</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="projection">�v���W�F�N�V�����s��</param>
	void XM_CALLCONV SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)override;

	/// <summary>
	/// �e�����󂯂�{�[���̐�(����͏����Ȃ�)
	/// </summary>
	/// <param name="value">�e���{�[����</param>
	/// <returns></returns>
	void __cdecl SetWeightsPerVertex(int value)override;

	/// <summary>
	/// �{�[���ϊ��s��̐ݒ�
	/// </summary>
	/// <param name="value">�ݒ肷��s��</param>
	/// <param name="count">���{�[����</param>
	/// <returns></returns>
	void __cdecl SetBoneTransforms(_In_reads_(count) XMMATRIX const* value, size_t count)override;

	/// <summary>
	/// �{�[���ϊ��s��̃��Z�b�g
	/// </summary>
	/// <returns></returns>
	void __cdecl ResetBoneTransforms()override;

	/// <summary>
	/// �X�|�b�g���C�g�̍��W�ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetLightPosition(Vector3 position);

	/// <summary>
	/// �X�|�b�g���C�g�̌����ݒ�
	/// </summary>
	/// <param name="direction"></param>
	void SetLightDirection(Vector3 direction);

	/// <summary>
	/// �ڂ̍��W�ݒ�
	/// </summary>
	/// <param name="eyePosition"></param>
	void SetEyePosition(Vector3 eyePosition);

	/// <summary>
	/// ���C�g�̃r���[��ԍs��ݒ�
	/// </summary>
	/// <param name="view"></param>
	void SetLightView(Matrix view);

private:
	PixelType m_type;		//�s�N�Z���V�F�[�_�[�̃^�C�v

	ComPtr<ID3D11VertexShader> m_vs;				//���_�V�F�[�_�[
	ComPtr<ID3D11PixelShader> m_ps;					//�s�N�Z���V�F�[�_�[
	ComPtr<ID3D11PixelShader> m_objectShadow;		//�I�u�W�F�N�g�[�x�V���h�E�p�s�N�Z���V�F�[�_�[
	ComPtr<ID3D11PixelShader> m_red;
	ComPtr<ID3D11PixelShader> m_blue;
	vector<uint8_t> m_vsBlob;						//���_�V�F�[�_�[�̃f�[�^���
	ComPtr<ID3D11ShaderResourceView> m_texture;		//�e�N�X�`��SRV
	ComPtr<ID3D11ShaderResourceView> m_normal;		//�@���}�b�vSRV
	ComPtr<ID3D11ShaderResourceView> m_ao;			//AO�}�b�vSRV

	Matrix m_world;			//���[���h�s��
	Matrix m_view;			//�r���[�s��
	Matrix m_projection;	//�v���W�F�N�V�����s��

	/// <summary>
	/// �萔�o�b�t�@�p�̍\����
	/// </summary>
	struct __declspec(align(16)) MatrixConstants
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	ConstantBuffer<MatrixConstants> m_matrixBuffer;	//�s��̒萔�o�b�t�@

	struct __declspec(align(16)) LightConstants
	{
		Vector3 direction;
		float range;
		Vector3 position;
		float angle;
		Vector3 eyePosition;
		float pad0;
	};
	
	LightConstants m_light;
	ConstantBuffer<LightConstants> m_lightBuffer;

	struct __declspec(align(16)) LVPConstants
	{
		XMMATRIX LVP;
	};
	ConstantBuffer<LVPConstants> m_LVPBuffer;
	Matrix m_lightView;

	/// <summary>
	/// �X�L�j���O�p�̒萔�o�b�t�@�p�\����
	/// </summary>
	struct __declspec(align(16)) SkinnedConstants
	{
		XMVECTOR bones[MaxBones][3];
	};
	SkinnedConstants m_skinnedConstants;
	ConstantBuffer<SkinnedConstants> m_skinnedBuffer;	//�{�[���ϊ��s��̒萔�o�b�t�@
};

