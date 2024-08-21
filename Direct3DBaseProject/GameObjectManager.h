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

	void ClearShadow();

	void SetShadowRenderTarget();

	void SetShadowResource();

	void DrawShadow();

private:
	static GameObjectManager* m_instance;	//マネージャのインスタンス

	vector<GameObject*> m_gameObjects;		//ゲームオブジェクトのコンテナ

	ComPtr<ID3D11Texture2D> m_shadowDepth;
	ComPtr<ID3D11DepthStencilView> m_shadowDepthView;
	ComPtr<ID3D11ShaderResourceView> m_shadowView;
};

