#pragma once
using namespace std;
using namespace Microsoft::WRL;

class GameObject;

/// <summary>
/// ゲームオブジェクト管理クラス
/// </summary>
class GameObjectManager
{
private:
	/// <summary>
	/// 管理するオブジェクトの設定
	/// </summary>
	GameObjectManager();

	/// <summary>
	/// データ破棄
	/// </summary>
	~GameObjectManager();
public:
	/// <summary>
	/// インスタンス生成
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// インスタンス破棄
	/// </summary>
	static void DestroyInstance();

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static GameObjectManager* GetInstance() { return m_instance; }

	/// <summary>
	/// オブジェクトの更新
	/// </summary>
	void Update();

	/// <summary>
	/// オブジェクトの描画
	/// </summary>
	void Draw();

	SpriteBatch* GetSpriteBatch() { return m_batch.get(); }

	void ClearObjectShadow();
	void ClearCharacterShadow();

	void SetObjectShadowRenderTarget();
	void SetCharacterShadowRenderTarget();

	void SetObjectShadowResource();
	void SetCharacterShadowResource();

	void DrawObjectShadow();
	void DrawCharacterShadow();

	void ClearHitCheckRenderTarget();
	void ClearHitCheckCharacterRenderTarget();

	void SetHitCheckRenderTarget();
	void SetHitCheckCharacterRenderTarget();

	void SetHitCheckShaderResource();
	void SetHitCheckCharacterShaderResource();

	void DrawHitCheck();
	void DrawHitCheckCharacter();

	void HitCheck();

private:
	static GameObjectManager* m_instance;	//マネージャのインスタンス

	vector<GameObject*> m_gameObjects;		//ゲームオブジェクトのコンテナ

	unique_ptr<SpriteBatch> m_batch;

	ComPtr<ID3D11Texture2D> m_objectShadowDepth;
	ComPtr<ID3D11Texture2D> m_characterShadowDepth;
	ComPtr<ID3D11DepthStencilView> m_objectShadowDepthView;
	ComPtr<ID3D11DepthStencilView> m_characterShadowDepthView;
	ComPtr<ID3D11ShaderResourceView> m_objectShadowView;
	ComPtr<ID3D11ShaderResourceView> m_characterShadowView;

	ComPtr<ID3D11RenderTargetView> m_hitCheckRenderTargetView;
	ComPtr<ID3D11RenderTargetView> m_hitCheckCharacterRTV;
	ComPtr<ID3D11DepthStencilView> m_hitCheckDSV;
	ComPtr<ID3D11ShaderResourceView> m_hitCheckShaderResourceView;
	ComPtr<ID3D11ShaderResourceView> m_hitCheckCharacterSRV;
};

