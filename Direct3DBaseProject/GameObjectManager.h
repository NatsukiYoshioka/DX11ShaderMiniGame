#pragma once
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
	/// �I�u�W�F�N�g�̍X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �I�u�W�F�N�g�̕`��
	/// </summary>
	void Draw();

	/// <summary>
	/// UI�`��p�f�o�C�X�̎擾
	/// </summary>
	/// <returns></returns>
	SpriteBatch* GetSpriteBatch() { return m_batch.get(); }

	/// <summary>
	/// �I�u�W�F�N�g�̉e�̏�����
	/// </summary>
	void ClearObjectShadow();

	/// <summary>
	/// �v���C���[�̉e�̏�����
	/// </summary>
	void ClearCharacterShadow();

	/// <summary>
	/// �I�u�W�F�N�g�̉e�̃����_�����O�^�[�Q�b�g��ݒ�
	/// </summary>
	void SetObjectShadowRenderTarget();

	/// <summary>
	/// �v���C���[�̉e�̃����_�����O�^�[�Q�b�g��ݒ�
	/// </summary>
	void SetCharacterShadowRenderTarget();

	/// <summary>
	/// �쐬�����I�u�W�F�N�g��SRV��ݒ�
	/// </summary>
	void SetObjectShadowResource();

	/// <summary>
	/// �쐬�����v���C���[��SRV��ݒ�
	/// </summary>
	void SetCharacterShadowResource();

	/// <summary>
	/// �I�u�W�F�N�g�̉e�`��
	/// </summary>
	void DrawObjectShadow();

	/// <summary>
	/// �v���C���[�̉e�`��
	/// </summary>
	void DrawCharacterShadow();

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

private:
	static GameObjectManager* m_instance;	//�}�l�[�W���̃C���X�^���X

	vector<GameObject*> m_gameObjects;		//�Q�[���I�u�W�F�N�g�̃R���e�i

	unique_ptr<SpriteBatch> m_batch;

	ComPtr<ID3D11Texture2D> m_objectShadowDepth;				//�I�u�W�F�N�g�̃e�N�X�`��
	ComPtr<ID3D11Texture2D> m_characterShadowDepth;				//�v���C���[�̃e�N�X�`��
	ComPtr<ID3D11DepthStencilView> m_objectShadowDepthView;		//�I�u�W�F�N�g�̐[�x�e�X�g�f�o�C�X
	ComPtr<ID3D11DepthStencilView> m_characterShadowDepthView;	//�v���C���[�̐[�x�e�X�g�f�o�C�X
	ComPtr<ID3D11ShaderResourceView> m_objectShadowView;		//�I�u�W�F�N�g�̃e�N�X�`��SRV
	ComPtr<ID3D11ShaderResourceView> m_characterShadowView;		//�v���C���[�̃e�N�X�`��SRV

	ComPtr<ID3D11RenderTargetView> m_hitCheckRenderTargetView;		//�����蔻��p�I�u�W�F�N�g�����_�[�^�[�Q�b�g
	ComPtr<ID3D11RenderTargetView> m_hitCheckCharacterRTV;			//�����蔻��L�����N�^�[�p�����_�[�^�[�Q�b�g
	ComPtr<ID3D11DepthStencilView> m_hitCheckDSV;					//�����蔻��p�[�x�e�X�g�f�o�C�X
	ComPtr<ID3D11ShaderResourceView> m_hitCheckShaderResourceView;	//�����蔻��p�I�u�W�F�N�gSRV
	ComPtr<ID3D11ShaderResourceView> m_hitCheckCharacterSRV;		//�����蔻��p�v���C���[SRV
};

