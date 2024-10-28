#pragma once
using namespace DirectX;
using namespace SimpleMath;
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
	/// 特定範囲でランダムな値を出力
	/// </summary>
	float RandomFloat(float min,float max);

	/// <summary>
	/// Lerp処理
	/// </summary>
	float Lerp(float a, float b, float t);

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
	/// タイトルシーンオブジェクトの初期化
	/// </summary>
	void InitializeTitle();

	/// <summary>
	/// タイトルシーンオブジェクトの更新
	/// </summary>
	void UpdateTitle();

	/// <summary>
	/// タイトルシーンオブジェクトの描画
	/// </summary>
	void DrawTitle();

	/// <summary>
	/// オブジェクトの描画
	/// </summary>
	void Initialize();

	/// <summary>
	/// オブジェクトの更新
	/// </summary>
	void Update();

	/// <summary>
	/// オブジェクトの描画
	/// </summary>
	void Draw();

	/// <summary>
	/// リザルトシーンオブジェクトの初期化
	/// </summary>
	void InitializeResult();

	/// <summary>
	/// リザルトシーンオブジェクトの更新
	/// </summary>
	void UpdateResult();

	/// <summary>
	/// リザルトシーンオブジェクトの描画
	/// </summary>
	void DrawResult();

	/// <summary>
	/// UI描画用デバイスの取得
	/// </summary>
	SpriteBatch* GetSpriteBatch() { return m_batch.get(); }

	/// <summary>
	/// オブジェクトの影の初期化
	/// </summary>
	void ClearObjectShadow();

	/// <summary>
	/// オブジェクトの影のレンダリングターゲットを設定
	/// </summary>
	void SetObjectShadowRenderTarget();

	/// <summary>
	/// 作成したオブジェクトのSRVを設定
	/// </summary>
	void SetObjectShadowResource();

	/// <summary>
	/// 影の描画
	/// </summary>
	void DrawShadow();

	/// <summary>
	/// SSAO描画(ポストプロセス)
	/// </summary>
	void DrawAmbientOcclusion();

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

	/// <summary>
	/// LUT描画(ポストプロセス)
	/// </summary>
	void DrawLUT();

	/// <summary>
	/// 法線・深度値を取得するためのRTVを取得
	/// </summary>
	ID3D11RenderTargetView* GetNormalDepthRTV() { return m_normalDepthRTV.Get(); }

	/// <summary>
	/// カラーグレーディングするための画面の色サンプリング用RTVを取得
	/// </summary>
	ID3D11RenderTargetView* GetLUTColorRTV() { return m_LUTColorRTV.Get(); }

private:
	static GameObjectManager* m_instance;	//マネージャのインスタンス

	vector<GameObject*> m_gameObjects;		//ゲームオブジェクトのコンテナ

	unique_ptr<SpriteBatch> m_batch;		//スプライト描画用バッチ

	/// <summary>
	/// 影描画用デバイスの作成
	/// </summary>
	void CreateShadowDevice();

	ComPtr<ID3D11Texture2D> m_objectShadowDepth;				//オブジェクトのテクスチャ
	ComPtr<ID3D11DepthStencilView> m_objectShadowDepthView;		//オブジェクトの深度テストデバイス
	ComPtr<ID3D11ShaderResourceView> m_objectShadowView;		//オブジェクトのテクスチャSRV

	/// <summary>
	/// 見つかり判定用デバイスの作成
	/// </summary>
	void CreateFindCheckDevice();

	ComPtr<ID3D11RenderTargetView> m_hitCheckRenderTargetView;		//見つかり判定用オブジェクトレンダーターゲット
	ComPtr<ID3D11RenderTargetView> m_hitCheckCharacterRTV;			//見つかり判定キャラクター用レンダーターゲット
	ComPtr<ID3D11DepthStencilView> m_hitCheckDSV;					//見つかり判定用深度テストデバイス
	ComPtr<ID3D11ShaderResourceView> m_hitCheckShaderResourceView;	//見つかり判定用オブジェクトSRV
	ComPtr<ID3D11ShaderResourceView> m_hitCheckCharacterSRV;		//見つかり判定用プレイヤーSRV

	ComPtr<ID3D11PixelShader> m_spritePixel;			//スプライト描画用ピクセルシェーダー

	/// <summary>
	/// SSAO描画用デバイスの作成
	/// </summary>
	void CreateAmbientOcclusionDevice();

	ComPtr<ID3D11Texture2D> m_normalDepthTexture;		//法線/深度値を参照するためのテクスチャ
	ComPtr<ID3D11RenderTargetView> m_normalDepthRTV;	//法線/深度値を入力するためのRTV
	ComPtr<ID3D11ShaderResourceView> m_normalDepthSRV;	//法線/深度値を参照するためのリソースビュー

	/// <summary>
	/// AO描画用定数構造体
	/// </summary>
	struct __declspec(align(16)) AOConstants
	{
		XMMATRIX projection;		//射影行列
		XMMATRIX inverseProjection;	//射影行列の逆行列
		Vector4 sampleKernel[64];	//AO用のサンプルカーネル
		float radius;				//AO描画用の半径
		float ZFar;					//AO描画用の最大深度値
		float AOPower;				//AOの強さ
	};
	ConstantBuffer<AOConstants> m_AOConstantBuffer;		//AO描画用定数バッファ

	/// <summary>
	/// ポストプロセッシング用の入力構造体
	/// </summary>
	struct Vertex
	{
		XMFLOAT3 Position;	//ポリゴンの座標
		XMFLOAT2 TexCoord;	//UV座標
	};
	ComPtr<ID3D11Buffer> m_vertexBuffer;				//ポストプロセッシング用頂点バッファ
	ComPtr<ID3D11Buffer> m_indexBuffer;					//ポストプロセッシング用インデックスバッファ
	ComPtr<ID3D11InputLayout> m_inputLayout;			//ポストプロセッシング用入力レイアウト
	ComPtr<ID3D11BlendState> m_AOBlend;					//AOのブレンド状態
	ComPtr<ID3D11VertexShader> m_PostProccessVertex;	//ポストプロセス用頂点シェーダー
	ComPtr<ID3D11PixelShader> m_AOPixel;				//AO用ピクセルシェーダー

	/// <summary>
	/// LUT用デバイスの作成
	/// </summary>
	void CreateLUTDevice();

	/// <summary>
	/// LUT用定数構造体
	/// </summary>
	struct __declspec(align(16)) LUTConstants
	{
		float gradingPower;		//カラーグレーディングの強さ
	};
	ConstantBuffer<LUTConstants> m_LUTConstantBuffer;		//LUT用定数バッファ

	ComPtr<ID3D11Texture2D> m_LUTColorTexture;			//LUT用カラーテクスチャ
	ComPtr<ID3D11RenderTargetView> m_LUTColorRTV;		//LUT用カラー入力RTV
	ComPtr<ID3D11SamplerState> m_LUTSampler;			//LUT用サンプラー
	ComPtr<ID3D11ShaderResourceView> m_LUTColorSRV;		//LUT用カラーリソースビュー
	ComPtr<ID3D11ShaderResourceView> m_LUTSampleSRV;	//LUT用サンプリングリソースビュー
	ComPtr<ID3D11PixelShader> m_LUTPixel;				//LUT用ピクセルシェーダー
};

