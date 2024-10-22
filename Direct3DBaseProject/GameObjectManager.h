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
	float RandomFloat(float min,float max);

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
	/// <returns></returns>
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

	void DrawShadow();

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

	void DrawLUT();

	ID3D11RenderTargetView* GetNormalDepthRTV() { return m_normalDepthRTV.Get(); }

	ID3D11RenderTargetView* GetLUTColorRTV() { return m_LUTColorRTV.Get(); }

private:
	static GameObjectManager* m_instance;	//マネージャのインスタンス

	vector<GameObject*> m_gameObjects;		//ゲームオブジェクトのコンテナ

	unique_ptr<SpriteBatch> m_batch;

	void CreateShadowDevice();

	ComPtr<ID3D11Texture2D> m_objectShadowDepth;				//オブジェクトのテクスチャ
	ComPtr<ID3D11Texture2D> m_characterShadowDepth;				//プレイヤーのテクスチャ
	ComPtr<ID3D11DepthStencilView> m_objectShadowDepthView;		//オブジェクトの深度テストデバイス
	ComPtr<ID3D11DepthStencilView> m_characterShadowDepthView;	//プレイヤーの深度テストデバイス
	ComPtr<ID3D11ShaderResourceView> m_objectShadowView;		//オブジェクトのテクスチャSRV
	ComPtr<ID3D11ShaderResourceView> m_characterShadowView;		//プレイヤーのテクスチャSRV

	void CreateFindCheckDevice();

	ComPtr<ID3D11RenderTargetView> m_hitCheckRenderTargetView;		//見つかり判定用オブジェクトレンダーターゲット
	ComPtr<ID3D11RenderTargetView> m_hitCheckCharacterRTV;			//見つかり判定キャラクター用レンダーターゲット
	ComPtr<ID3D11DepthStencilView> m_hitCheckDSV;					//見つかり判定用深度テストデバイス
	ComPtr<ID3D11ShaderResourceView> m_hitCheckShaderResourceView;	//見つかり判定用オブジェクトSRV
	ComPtr<ID3D11ShaderResourceView> m_hitCheckCharacterSRV;		//見つかり判定用プレイヤーSRV

	ComPtr<ID3D11PixelShader> m_spritePixel;

	void CreateAmbientOcclusionDevice();

	ComPtr<ID3D11Texture2D> m_normalDepthTexture;
	ComPtr<ID3D11RenderTargetView> m_normalDepthRTV;
	ComPtr<ID3D11ShaderResourceView> m_normalDepthSRV;

	struct __declspec(align(16)) AOConstants
	{
		XMMATRIX projection;
		XMMATRIX inverseProjection;
		Vector4 sampleKernel[64];
		float radius;
		float ZFar;
		float AOPower;
	};
	ConstantBuffer<AOConstants> m_AOConstantBuffer;
	struct Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT2 TexCoord;
	};
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	ComPtr<ID3D11InputLayout> m_inputLayout;
	ComPtr<ID3D11BlendState> m_AOBlend;
	ComPtr<ID3D11VertexShader> m_PostProccessVertex;
	ComPtr<ID3D11PixelShader> m_AOPixel;

	void CreateLUTDevice();

	struct __declspec(align(16)) LUTConstants
	{
		float gradingPower;
	};
	ConstantBuffer<LUTConstants> m_LUTConstantBuffer;

	ComPtr<ID3D11Texture2D> m_LUTColorTexture;
	ComPtr<ID3D11RenderTargetView> m_LUTColorRTV;
	ComPtr<ID3D11ShaderResourceView> m_LUTColorSRV;
	ComPtr<ID3D11ShaderResourceView> m_LUTSampleSRV;
	ComPtr<ID3D11PixelShader> m_LUTPixel;
};

