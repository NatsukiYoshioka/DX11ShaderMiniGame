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
	/// エフェクトの初期化
	/// </summary>
	/// <param name="device">描画デバイス</param>
	explicit OriginalEffect(ID3D11Device* device, bool isSkinning = false);

	/// <summary>
	/// エフェクトの適用処理
	/// </summary>
	/// <param name="context">描画コンテキスト</param>
	virtual void Apply(ID3D11DeviceContext* context);

	/// <summary>
	/// 頂点シェーダーのバイトコード取得
	/// </summary>
	/// <param name="pShaderBytecode">バイトコード</param>
	/// <param name="pBytecodeLength">バイトコードのサイズ</param>
	virtual void GetVertexShaderBytecode(void const** pShaderBytecode, size_t* pBytecodeLength)override;

	/// <summary>
	/// テクスチャの設定
	/// </summary>
	/// <param name="value">テクスチャのSRV</param>
	void SetTexture(ID3D11ShaderResourceView* value);

	/// <summary>
	/// 法線マップの設定
	/// </summary>
	/// <param name="value">法線マップのSRV</param>
	void SetNormal(ID3D11ShaderResourceView* value);

	/// <summary>
	/// AOマップの設定
	/// </summary>
	/// <param name="value">AOマップのSRV</param>
	void SetAO(ID3D11ShaderResourceView* value);

	/// <summary>
	/// ワールド行列の設定
	/// </summary>
	/// <param name="world">ワールド行列</param>
	void XM_CALLCONV SetWorld(FXMMATRIX world)override;

	/// <summary>
	/// ビュー行列の設定
	/// </summary>
	/// <param name="view">ビュー行列</param>
	void XM_CALLCONV SetView(FXMMATRIX view)override;

	/// <summary>
	/// プロジェクション行列の設定
	/// </summary>
	/// <param name="projection">プロジェクション行列</param>
	/// <returns></returns>
	void XM_CALLCONV SetProjection(FXMMATRIX projection)override;

	/// <summary>
	/// ワールド,ビュー,ロジェクション行列の設定
	/// </summary>
	/// <param name="world">ワールド行列</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="projection">プロジェクション行列</param>
	void XM_CALLCONV SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)override;

	/// <summary>
	/// 影響を受けるボーンの数(今回は処理なし)
	/// </summary>
	/// <param name="value">影響ボーン数</param>
	/// <returns></returns>
	void __cdecl SetWeightsPerVertex(int value)override;

	/// <summary>
	/// ボーン変換行列の設定
	/// </summary>
	/// <param name="value">設定する行列</param>
	/// <param name="count">総ボーン数</param>
	/// <returns></returns>
	void __cdecl SetBoneTransforms(_In_reads_(count) XMMATRIX const* value, size_t count)override;

	/// <summary>
	/// ボーン変換行列のリセット
	/// </summary>
	/// <returns></returns>
	void __cdecl ResetBoneTransforms()override;

private:
	ComPtr<ID3D11VertexShader> m_vs;			//頂点シェーダー
	ComPtr<ID3D11PixelShader> m_ps;				//ピクセルシェーダー
	vector<uint8_t> m_vsBlob;					//頂点シェーダーのデータ情報
	ComPtr<ID3D11ShaderResourceView> m_texture;	//テクスチャSRV
	ComPtr<ID3D11ShaderResourceView> m_normal;	//法線マップSRV
	ComPtr<ID3D11ShaderResourceView> m_ao;		//AOマップSRV

	Matrix m_world;			//ワールド行列
	Matrix m_view;			//ビュー行列
	Matrix m_projection;	//プロジェクション行列

	uint32_t m_dirtyFlags;

	/// <summary>
	/// 定数バッファ用の構造体
	/// </summary>
	struct __declspec(align(16)) MatrixConstants
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	ConstantBuffer<MatrixConstants> m_matrixBuffer;	//行列の定数バッファ

	/// <summary>
	/// スキニング用の定数バッファ用構造体
	/// </summary>
	struct __declspec(align(16)) SkinnedConstants
	{
		XMVECTOR bones[MaxBones][3];
	};
	SkinnedConstants m_skinnedConstants;
	ConstantBuffer<SkinnedConstants> m_skinnedBuffer;	//ボーン変換行列の定数バッファ
};

