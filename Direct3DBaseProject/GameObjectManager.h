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

	void ClearObjectShadow();
	void ClearCharacterShadow();

	void SetObjectShadowRenderTarget();
	void SetCharacterShadowRenderTarget();

	void SetObjectShadowResource();
	void SetCharacterShadowResource();

	void DrawObjectShadow();
	void DrawCharacterShadow();

private:
	static GameObjectManager* m_instance;	//�}�l�[�W���̃C���X�^���X

	vector<GameObject*> m_gameObjects;		//�Q�[���I�u�W�F�N�g�̃R���e�i

	ComPtr<ID3D11Texture2D> m_objectShadowDepth;
	ComPtr<ID3D11Texture2D> m_characterShadowDepth;
	ComPtr<ID3D11DepthStencilView> m_objectShadowDepthView;
	ComPtr<ID3D11DepthStencilView> m_characterShadowDepthView;
	ComPtr<ID3D11ShaderResourceView> m_objectShadowView;
	ComPtr<ID3D11ShaderResourceView> m_characterShadowView;

	ComPtr<ID3D11RenderTargetView> m_hitCheckRenderTargetView;
	ComPtr<ID3D11ShaderResourceView> m_hitCheckShaderResourceView;
};

