#pragma once
using namespace DirectX;
using namespace SimpleMath;
using namespace std;
using namespace Microsoft::WRL;

class GameObject;

/// <summary>
/// �Q�[���I�u�W�F�N�g�Ǘ��N���X
/// </summary>
class GameObjectManager
{
private:
	/// <summary>
	/// ����͈͂Ń����_���Ȓl���o��
	/// </summary>
	float RandomFloat(float min,float max);

	/// <summary>
	/// Lerp����
	/// </summary>
	float Lerp(float a, float b, float t);

	/// <summary>
	/// �Ǘ�����I�u�W�F�N�g�̐ݒ�
	/// </summary>
	GameObjectManager();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~GameObjectManager();
public:
	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// �C���X�^���X�j��
	/// </summary>
	static void DestroyInstance();

	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns></returns>
	static GameObjectManager* GetInstance() { return m_instance; }

	/// <summary>
	/// �^�C�g���V�[���I�u�W�F�N�g�̏�����
	/// </summary>
	void InitializeTitle();

	/// <summary>
	/// �^�C�g���V�[���I�u�W�F�N�g�̍X�V
	/// </summary>
	void UpdateTitle();

	/// <summary>
	/// �^�C�g���V�[���I�u�W�F�N�g�̕`��
	/// </summary>
	void DrawTitle();

	/// <summary>
	/// �I�u�W�F�N�g�̕`��
	/// </summary>
	void Initialize();

	/// <summary>
	/// �I�u�W�F�N�g�̍X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �I�u�W�F�N�g�̕`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���U���g�V�[���I�u�W�F�N�g�̏�����
	/// </summary>
	void InitializeResult();

	/// <summary>
	/// ���U���g�V�[���I�u�W�F�N�g�̍X�V
	/// </summary>
	void UpdateResult();

	/// <summary>
	/// ���U���g�V�[���I�u�W�F�N�g�̕`��
	/// </summary>
	void DrawResult();

	/// <summary>
	/// UI�`��p�f�o�C�X�̎擾
	/// </summary>
	SpriteBatch* GetSpriteBatch() { return m_batch.get(); }

	/// <summary>
	/// �I�u�W�F�N�g�̉e�̏�����
	/// </summary>
	void ClearObjectShadow();

	/// <summary>
	/// �I�u�W�F�N�g�̉e�̃����_�����O�^�[�Q�b�g��ݒ�
	/// </summary>
	void SetObjectShadowRenderTarget();

	/// <summary>
	/// �쐬�����I�u�W�F�N�g��SRV��ݒ�
	/// </summary>
	void SetObjectShadowResource();

	/// <summary>
	/// �e�̕`��
	/// </summary>
	void DrawShadow();

	/// <summary>
	/// SSAO�`��(�|�X�g�v���Z�X)
	/// </summary>
	void DrawAmbientOcclusion();

	/// <summary>
	/// �G���_�̌����蔻��p�����_�����O�^�[�Q�b�g������
	/// </summary>
	void ClearHitCheckRenderTarget();

	/// <summary>
	/// �G���_�̌����蔻��p�v���C���[�����_�����O�^�[�Q�b�g������
	/// </summary>
	void ClearHitCheckCharacterRenderTarget();

	/// <summary>
	/// �G���_�̌����蔻��p�����_�����O�^�[�Q�b�g�ݒ�
	/// </summary>
	void SetHitCheckRenderTarget();

	/// <summary>
	/// �G���_�̌����蔻��p�v���C���[�����_�����O�^�[�Q�b�g�ݒ�
	/// </summary>
	void SetHitCheckCharacterRenderTarget();

	/// <summary>
	/// �����蔻��p�e�N�X�`��SRV�ݒ�
	/// </summary>
	void SetHitCheckShaderResource();

	/// <summary>
	/// �����蔻��p�v���C���[�e�N�X�`��SRV�ݒ�
	/// </summary>
	void SetHitCheckCharacterShaderResource();

	/// <summary>
	/// �����蔻��p�I�u�W�F�N�g�`��
	/// </summary>
	void DrawHitCheck();

	/// <summary>
	/// �����蔻��p�v���C���[�`��
	/// </summary>
	void DrawHitCheckCharacter();

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	void HitCheck();

	/// <summary>
	/// LUT�`��(�|�X�g�v���Z�X)
	/// </summary>
	void DrawLUT();

	/// <summary>
	/// �@���E�[�x�l���擾���邽�߂�RTV���擾
	/// </summary>
	ID3D11RenderTargetView* GetNormalDepthRTV() { return m_normalDepthRTV.Get(); }

	/// <summary>
	/// �J���[�O���[�f�B���O���邽�߂̉�ʂ̐F�T���v�����O�pRTV���擾
	/// </summary>
	ID3D11RenderTargetView* GetLUTColorRTV() { return m_LUTColorRTV.Get(); }

private:
	static GameObjectManager* m_instance;	//�}�l�[�W���̃C���X�^���X

	vector<GameObject*> m_gameObjects;		//�Q�[���I�u�W�F�N�g�̃R���e�i

	unique_ptr<SpriteBatch> m_batch;		//�X�v���C�g�`��p�o�b�`

	/// <summary>
	/// �e�`��p�f�o�C�X�̍쐬
	/// </summary>
	void CreateShadowDevice();

	ComPtr<ID3D11Texture2D> m_objectShadowDepth;				//�I�u�W�F�N�g�̃e�N�X�`��
	ComPtr<ID3D11DepthStencilView> m_objectShadowDepthView;		//�I�u�W�F�N�g�̐[�x�e�X�g�f�o�C�X
	ComPtr<ID3D11ShaderResourceView> m_objectShadowView;		//�I�u�W�F�N�g�̃e�N�X�`��SRV

	/// <summary>
	/// �����蔻��p�f�o�C�X�̍쐬
	/// </summary>
	void CreateFindCheckDevice();

	ComPtr<ID3D11RenderTargetView> m_hitCheckRenderTargetView;		//�����蔻��p�I�u�W�F�N�g�����_�[�^�[�Q�b�g
	ComPtr<ID3D11RenderTargetView> m_hitCheckCharacterRTV;			//�����蔻��L�����N�^�[�p�����_�[�^�[�Q�b�g
	ComPtr<ID3D11DepthStencilView> m_hitCheckDSV;					//�����蔻��p�[�x�e�X�g�f�o�C�X
	ComPtr<ID3D11ShaderResourceView> m_hitCheckShaderResourceView;	//�����蔻��p�I�u�W�F�N�gSRV
	ComPtr<ID3D11ShaderResourceView> m_hitCheckCharacterSRV;		//�����蔻��p�v���C���[SRV

	ComPtr<ID3D11PixelShader> m_spritePixel;			//�X�v���C�g�`��p�s�N�Z���V�F�[�_�[

	/// <summary>
	/// SSAO�`��p�f�o�C�X�̍쐬
	/// </summary>
	void CreateAmbientOcclusionDevice();

	ComPtr<ID3D11Texture2D> m_normalDepthTexture;		//�@��/�[�x�l���Q�Ƃ��邽�߂̃e�N�X�`��
	ComPtr<ID3D11RenderTargetView> m_normalDepthRTV;	//�@��/�[�x�l����͂��邽�߂�RTV
	ComPtr<ID3D11ShaderResourceView> m_normalDepthSRV;	//�@��/�[�x�l���Q�Ƃ��邽�߂̃��\�[�X�r���[

	/// <summary>
	/// AO�`��p�萔�\����
	/// </summary>
	struct __declspec(align(16)) AOConstants
	{
		XMMATRIX projection;		//�ˉe�s��
		XMMATRIX inverseProjection;	//�ˉe�s��̋t�s��
		Vector4 sampleKernel[64];	//AO�p�̃T���v���J�[�l��
		float radius;				//AO�`��p�̔��a
		float ZFar;					//AO�`��p�̍ő�[�x�l
		float AOPower;				//AO�̋���
	};
	ConstantBuffer<AOConstants> m_AOConstantBuffer;		//AO�`��p�萔�o�b�t�@

	/// <summary>
	/// �|�X�g�v���Z�b�V���O�p�̓��͍\����
	/// </summary>
	struct Vertex
	{
		XMFLOAT3 Position;	//�|���S���̍��W
		XMFLOAT2 TexCoord;	//UV���W
	};
	ComPtr<ID3D11Buffer> m_vertexBuffer;				//�|�X�g�v���Z�b�V���O�p���_�o�b�t�@
	ComPtr<ID3D11Buffer> m_indexBuffer;					//�|�X�g�v���Z�b�V���O�p�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D11InputLayout> m_inputLayout;			//�|�X�g�v���Z�b�V���O�p���̓��C�A�E�g
	ComPtr<ID3D11BlendState> m_AOBlend;					//AO�̃u�����h���
	ComPtr<ID3D11VertexShader> m_PostProccessVertex;	//�|�X�g�v���Z�X�p���_�V�F�[�_�[
	ComPtr<ID3D11PixelShader> m_AOPixel;				//AO�p�s�N�Z���V�F�[�_�[

	/// <summary>
	/// LUT�p�f�o�C�X�̍쐬
	/// </summary>
	void CreateLUTDevice();

	/// <summary>
	/// LUT�p�萔�\����
	/// </summary>
	struct __declspec(align(16)) LUTConstants
	{
		float gradingPower;		//�J���[�O���[�f�B���O�̋���
	};
	ConstantBuffer<LUTConstants> m_LUTConstantBuffer;		//LUT�p�萔�o�b�t�@

	ComPtr<ID3D11Texture2D> m_LUTColorTexture;			//LUT�p�J���[�e�N�X�`��
	ComPtr<ID3D11RenderTargetView> m_LUTColorRTV;		//LUT�p�J���[����RTV
	ComPtr<ID3D11SamplerState> m_LUTSampler;			//LUT�p�T���v���[
	ComPtr<ID3D11ShaderResourceView> m_LUTColorSRV;		//LUT�p�J���[���\�[�X�r���[
	ComPtr<ID3D11ShaderResourceView> m_LUTSampleSRV;	//LUT�p�T���v�����O���\�[�X�r���[
	ComPtr<ID3D11PixelShader> m_LUTPixel;				//LUT�p�s�N�Z���V�F�[�_�[
};

