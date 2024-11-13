#pragma once

using namespace DirectX;
using namespace SimpleMath;
using namespace Microsoft::WRL;
using namespace std;

/// <summary>
/// モデル描画用自作エフェクトクラス
/// </summary>
class OriginalEffect :public IEffect, public IEffectMatrices, public IEffectSkinning
{
public:
	/// <summary>
	/// ピクセルシェーダーのタイプ
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
	/// ピクセルシェーダーのタイプ更新
	/// </summary>
	void UpdateType(PixelType value) { m_type = value; }

	/// <summary>
	/// エフェクトの初期化
	/// </summary>
	explicit OriginalEffect(ID3D11Device* device, PixelType type, bool isSkinning = false);

	/// <summary>
	/// エフェクトの適用処理
	/// </summary>
	virtual void Apply(ID3D11DeviceContext* context);

	/// <summary>
	/// 頂点シェーダーのバイトコード取得
	/// </summary>
	virtual void GetVertexShaderBytecode(void const** pShaderBytecode, size_t* pBytecodeLength)override;

	/// <summary>
	/// テクスチャの設定
	/// </summary>
	void SetTexture(ID3D11ShaderResourceView* value);

	/// <summary>
	/// ブロックのテクスチャ設定
	/// </summary>
	void SetBlockTexture(
		ID3D11ShaderResourceView* value1, 
		ID3D11ShaderResourceView* value2, 
		ID3D11ShaderResourceView* value3, 
		ID3D11ShaderResourceView* value4);

	/// <summary>
	/// ワールド行列の設定
	/// </summary>
	void XM_CALLCONV SetWorld(FXMMATRIX world)override;

	/// <summary>
	/// ビュー行列の設定
	/// </summary>
	void XM_CALLCONV SetView(FXMMATRIX view)override;

	/// <summary>
	/// プロジェクション行列の設定
	/// </summary>
	void XM_CALLCONV SetProjection(FXMMATRIX projection)override;

	/// <summary>
	/// ワールド,ビュー,ロジェクション行列の設定
	/// </summary>
	void XM_CALLCONV SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)override;

	/// <summary>
	/// 影響を受けるボーンの数(今回は処理なし)
	/// </summary>
	void __cdecl SetWeightsPerVertex(int value)override;

	/// <summary>
	/// ボーン変換行列の設定
	/// </summary>
	void __cdecl SetBoneTransforms(_In_reads_(count) XMMATRIX const* value, size_t count)override;

	/// <summary>
	/// ボーン変換行列のリセット
	/// </summary>
	void __cdecl ResetBoneTransforms()override;

	/// <summary>
	/// スポットライトの座標設定
	/// </summary>
	void SetLightPosition(Vector3 position);

	/// <summary>
	/// スポットライトの向き設定
	/// </summary>
	void SetLightDirection(Vector3 direction);

	/// <summary>
	/// 目の座標設定
	/// </summary>
	void SetEyePosition(Vector3 eyePosition);

	/// <summary>
	/// ライトの色設定
	/// </summary>
	void SetLightColor(Vector3 lightColor);

	/// <summary>
	/// ライトのビュー空間行列設定
	/// </summary>
	void SetLightView(Matrix view);

private:
	PixelType m_type;		//ピクセルシェーダーのタイプ

	ComPtr<ID3D11VertexShader> m_vs;				//頂点シェーダー
	ComPtr<ID3D11PixelShader> m_ps;					//ピクセルシェーダー
	ComPtr<ID3D11PixelShader> m_objectShadow;		//オブジェクト深度シャドウ用ピクセルシェーダー
	ComPtr<ID3D11PixelShader> m_blockShadow;		//ブロックの影用ピクセルシェーダー
	ComPtr<ID3D11PixelShader> m_red;				//赤染色ピクセルシェーダー
	ComPtr<ID3D11PixelShader> m_blue;				//青染色ピクセルシェーダー
	vector<uint8_t> m_vsBlob;						//頂点シェーダーのデータ情報
	ComPtr<ID3D11ShaderResourceView> m_texture1;		//テクスチャSRV
	ComPtr<ID3D11ShaderResourceView> m_texture2;		//テクスチャSRV
	ComPtr<ID3D11ShaderResourceView> m_texture3;		//テクスチャSRV
	ComPtr<ID3D11ShaderResourceView> m_texture4;		//テクスチャSRV

	Matrix m_world;			//ワールド行列
	Matrix m_view;			//ビュー行列
	Matrix m_projection;	//プロジェクション行列

	/// <summary>
	/// 定数バッファ用の構造体
	/// </summary>
	struct __declspec(align(16)) MatrixConstants
	{
		XMMATRIX world;			//ワールド行列
		XMMATRIX view;			//ビュー行列
		XMMATRIX projection;	//射影行列
	};
	ConstantBuffer<MatrixConstants> m_matrixBuffer;	//行列の定数バッファ

	/// <summary>
	/// ライト用定数構造体
	/// </summary>
	struct __declspec(align(16)) LightConstants
	{
		Vector3 direction = Vector3::Zero;		//ライトの向きベクトル
		float range = 0;						//ライトの範囲
		Vector3 position = Vector3::Zero;		//ライトの光源座標
		float angle = 0;						//ライトの向き
		Vector3 eyePosition = Vector3::Zero;	//目の座標
		float pad0 = 0;							//パディング
		Vector3 lightColor = Vector3::Zero;		//ライトの色
		float pad1 = 0;							//パディング
	};
	LightConstants m_light;							//ライト用定数構造体
	ConstantBuffer<LightConstants> m_lightBuffer;	//ライト用定数バッファ

	/// <summary>
	/// ライトビュープロジェクション行列定数
	/// </summary>
	struct __declspec(align(16)) LVPConstants
	{
		XMMATRIX LVP;
	};
	ConstantBuffer<LVPConstants> m_LVPBuffer;		//ライトビュープロジェクション行列定数バッファ
	Matrix m_lightView;								//ライトビュー行列

	/// <summary>
	/// スキニング用の定数バッファ用構造体
	/// </summary>
	struct __declspec(align(16)) SkinnedConstants
	{
		XMVECTOR bones[MaxBones][3];
	};
	SkinnedConstants m_skinnedConstants;				//ボーン変換行列
	ConstantBuffer<SkinnedConstants> m_skinnedBuffer;	//ボーン変換行列の定数バッファ
};

