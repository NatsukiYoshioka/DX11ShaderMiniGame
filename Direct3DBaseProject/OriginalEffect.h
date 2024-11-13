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
		Block,
		Character,
		Light,
		Shadow,
		BlockShadow,
		Red,
		Blue
	};

	/// <summary>
	/// �s�N�Z���V�F�[�_�[�̃^�C�v�X�V
	/// </summary>
	void UpdateType(PixelType value) { m_type = value; }

	/// <summary>
	/// �G�t�F�N�g�̏�����
	/// </summary>
	explicit OriginalEffect(ID3D11Device* device, PixelType type, bool isSkinning = false);

	/// <summary>
	/// �G�t�F�N�g�̓K�p����
	/// </summary>
	virtual void Apply(ID3D11DeviceContext* context);

	/// <summary>
	/// ���_�V�F�[�_�[�̃o�C�g�R�[�h�擾
	/// </summary>
	virtual void GetVertexShaderBytecode(void const** pShaderBytecode, size_t* pBytecodeLength)override;

	/// <summary>
	/// �e�N�X�`���̐ݒ�
	/// </summary>
	void SetTexture(ID3D11ShaderResourceView* value);

	/// <summary>
	/// �u���b�N�̃e�N�X�`���ݒ�
	/// </summary>
	void SetBlockTexture(
		ID3D11ShaderResourceView* value1, 
		ID3D11ShaderResourceView* value2, 
		ID3D11ShaderResourceView* value3, 
		ID3D11ShaderResourceView* value4);

	/// <summary>
	/// ���[���h�s��̐ݒ�
	/// </summary>
	void XM_CALLCONV SetWorld(FXMMATRIX world)override;

	/// <summary>
	/// �r���[�s��̐ݒ�
	/// </summary>
	void XM_CALLCONV SetView(FXMMATRIX view)override;

	/// <summary>
	/// �v���W�F�N�V�����s��̐ݒ�
	/// </summary>
	void XM_CALLCONV SetProjection(FXMMATRIX projection)override;

	/// <summary>
	/// ���[���h,�r���[,���W�F�N�V�����s��̐ݒ�
	/// </summary>
	void XM_CALLCONV SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)override;

	/// <summary>
	/// �e�����󂯂�{�[���̐�(����͏����Ȃ�)
	/// </summary>
	void __cdecl SetWeightsPerVertex(int value)override;

	/// <summary>
	/// �{�[���ϊ��s��̐ݒ�
	/// </summary>
	void __cdecl SetBoneTransforms(_In_reads_(count) XMMATRIX const* value, size_t count)override;

	/// <summary>
	/// �{�[���ϊ��s��̃��Z�b�g
	/// </summary>
	void __cdecl ResetBoneTransforms()override;

	/// <summary>
	/// �X�|�b�g���C�g�̍��W�ݒ�
	/// </summary>
	void SetLightPosition(Vector3 position);

	/// <summary>
	/// �X�|�b�g���C�g�̌����ݒ�
	/// </summary>
	void SetLightDirection(Vector3 direction);

	/// <summary>
	/// �ڂ̍��W�ݒ�
	/// </summary>
	void SetEyePosition(Vector3 eyePosition);

	/// <summary>
	/// ���C�g�̐F�ݒ�
	/// </summary>
	void SetLightColor(Vector3 lightColor);

	/// <summary>
	/// ���C�g�̃r���[��ԍs��ݒ�
	/// </summary>
	void SetLightView(Matrix view);

private:
	PixelType m_type;		//�s�N�Z���V�F�[�_�[�̃^�C�v

	ComPtr<ID3D11VertexShader> m_vs;				//���_�V�F�[�_�[
	ComPtr<ID3D11PixelShader> m_ps;					//�s�N�Z���V�F�[�_�[
	ComPtr<ID3D11PixelShader> m_objectShadow;		//�I�u�W�F�N�g�[�x�V���h�E�p�s�N�Z���V�F�[�_�[
	ComPtr<ID3D11PixelShader> m_blockShadow;		//�u���b�N�̉e�p�s�N�Z���V�F�[�_�[
	ComPtr<ID3D11PixelShader> m_red;				//�Ԑ��F�s�N�Z���V�F�[�_�[
	ComPtr<ID3D11PixelShader> m_blue;				//���F�s�N�Z���V�F�[�_�[
	vector<uint8_t> m_vsBlob;						//���_�V�F�[�_�[�̃f�[�^���
	ComPtr<ID3D11ShaderResourceView> m_texture1;		//�e�N�X�`��SRV
	ComPtr<ID3D11ShaderResourceView> m_texture2;		//�e�N�X�`��SRV
	ComPtr<ID3D11ShaderResourceView> m_texture3;		//�e�N�X�`��SRV
	ComPtr<ID3D11ShaderResourceView> m_texture4;		//�e�N�X�`��SRV

	Matrix m_world;			//���[���h�s��
	Matrix m_view;			//�r���[�s��
	Matrix m_projection;	//�v���W�F�N�V�����s��

	/// <summary>
	/// �萔�o�b�t�@�p�̍\����
	/// </summary>
	struct __declspec(align(16)) MatrixConstants
	{
		XMMATRIX world;			//���[���h�s��
		XMMATRIX view;			//�r���[�s��
		XMMATRIX projection;	//�ˉe�s��
	};
	ConstantBuffer<MatrixConstants> m_matrixBuffer;	//�s��̒萔�o�b�t�@

	/// <summary>
	/// ���C�g�p�萔�\����
	/// </summary>
	struct __declspec(align(16)) LightConstants
	{
		Vector3 direction = Vector3::Zero;		//���C�g�̌����x�N�g��
		float range = 0;						//���C�g�͈̔�
		Vector3 position = Vector3::Zero;		//���C�g�̌������W
		float angle = 0;						//���C�g�̌���
		Vector3 eyePosition = Vector3::Zero;	//�ڂ̍��W
		float pad0 = 0;							//�p�f�B���O
		Vector3 lightColor = Vector3::Zero;		//���C�g�̐F
		float pad1 = 0;							//�p�f�B���O
	};
	LightConstants m_light;							//���C�g�p�萔�\����
	ConstantBuffer<LightConstants> m_lightBuffer;	//���C�g�p�萔�o�b�t�@

	/// <summary>
	/// ���C�g�r���[�v���W�F�N�V�����s��萔
	/// </summary>
	struct __declspec(align(16)) LVPConstants
	{
		XMMATRIX LVP;
	};
	ConstantBuffer<LVPConstants> m_LVPBuffer;		//���C�g�r���[�v���W�F�N�V�����s��萔�o�b�t�@
	Matrix m_lightView;								//���C�g�r���[�s��

	/// <summary>
	/// �X�L�j���O�p�̒萔�o�b�t�@�p�\����
	/// </summary>
	struct __declspec(align(16)) SkinnedConstants
	{
		XMVECTOR bones[MaxBones][3];
	};
	SkinnedConstants m_skinnedConstants;				//�{�[���ϊ��s��
	ConstantBuffer<SkinnedConstants> m_skinnedBuffer;	//�{�[���ϊ��s��̒萔�o�b�t�@
};

