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

	/// <summary>
	/// UI描画用デバイスの取得
	/// </summary>
	/// <returns></returns>
	SpriteBatch* GetSpriteBatch() { return m_batch.get(); }

	/// <summary>
	/// オブジェクトの影の初期化
	/// </summary>
	void ClearObjectShadow();

	/// <summary>
	/// プレイヤーの影の初期化
	/// </summary>
	void ClearCharacterShadow();

	/// <summary>
	/// オブジェクトの影のレンダリングターゲットを設定
	/// </summary>
	void SetObjectShadowRenderTarget();

	/// <summary>
	/// プレイヤーの影のレンダリングターゲットを設定
	/// </summary>
	void SetCharacterShadowRenderTarget();

	/// <summary>
	/// 作成したオブジェクトのSRVを設定
	/// </summary>
	void SetObjectShadowResource();

	/// <summary>
	/// 作成したプレイヤーのSRVを設定
	/// </summary>
	void SetCharacterShadowResource();

	/// <summary>
	/// オブジェクトの影描画
	/// </summary>
	void DrawObjectShadow();

	/// <summary>
	/// プレイヤーの影描画
	/// </summary>
	void DrawCharacterShadow();

	/// <summary>
	/// 敵視点の見つかり判定用レンダリングターゲット初期化
	/// </summary>
	void ClearHitCheckRenderTarget();

	/// <summary>
	/// 敵視点の見つかり判定用プレイヤーレンダリングターゲット初期化
	/// </summary>
	void ClearHitCheckCharacterRenderTarget();

	/// <summary>
	/// 敵視点の見つかり判定用レンダリングターゲット設定
	/// </summary>
	void SetHitCheckRenderTarget();

	/// <summary>
	/// 敵視点の見つかり判定用プレイヤーレンダリングターゲット設定
	/// </summary>
	void SetHitCheckCharacterRenderTarget();

	/// <summary>
	/// 見つかり判定用テクスチャSRV設定
	/// </summary>
	void SetHitCheckShaderResource();

	/// <summary>
	/// 見つかり判定用プレイヤーテクスチャSRV設定
	/// </summary>
	void SetHitCheckCharacterShaderResource();

	/// <summary>
	/// 見つかり判定用オブジェクト描画
	/// </summary>
	void DrawHitCheck();

	/// <summary>
	/// 見つかり判定用プレイヤー描画
	/// </summary>
	void DrawHitCheckCharacter();

	/// <summary>
	/// 見つかり判定処理
	/// </summary>
	void HitCheck();

private:
	static GameObjectManager* m_instance;	//マネージャのインスタンス

	vector<GameObject*> m_gameObjects;		//ゲームオブジェクトのコンテナ

	unique_ptr<SpriteBatch> m_batch;

	ComPtr<ID3D11Texture2D> m_objectShadowDepth;				//オブジェクトのテクスチャ
	ComPtr<ID3D11Texture2D> m_characterShadowDepth;				//プレイヤーのテクスチャ
	ComPtr<ID3D11DepthStencilView> m_objectShadowDepthView;		//オブジェクトの深度テストデバイス
	ComPtr<ID3D11DepthStencilView> m_characterShadowDepthView;	//プレイヤーの深度テストデバイス
	ComPtr<ID3D11ShaderResourceView> m_objectShadowView;		//オブジェクトのテクスチャSRV
	ComPtr<ID3D11ShaderResourceView> m_characterShadowView;		//プレイヤーのテクスチャSRV

	ComPtr<ID3D11RenderTargetView> m_hitCheckRenderTargetView;		//見つかり判定用オブジェクトレンダーターゲット
	ComPtr<ID3D11RenderTargetView> m_hitCheckCharacterRTV;			//見つかり判定キャラクター用レンダーターゲット
	ComPtr<ID3D11DepthStencilView> m_hitCheckDSV;					//見つかり判定用深度テストデバイス
	ComPtr<ID3D11ShaderResourceView> m_hitCheckShaderResourceView;	//見つかり判定用オブジェクトSRV
	ComPtr<ID3D11ShaderResourceView> m_hitCheckCharacterSRV;		//見つかり判定用プレイヤーSRV
};

