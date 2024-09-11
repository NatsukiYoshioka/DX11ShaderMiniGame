#pragma once

using namespace Microsoft::WRL;

class UIBase;

/// <summary>
/// 見つかり判定UI
/// </summary>
class FoundUI:public UIBase
{
public:
	/// <summary>
	/// UI初期化
	/// </summary>
	FoundUI();

	/// <summary>
	/// データ破棄
	/// </summary>
	~FoundUI();

	/// <summary>
	/// タイトルシーンUI初期化
	/// </summary>
	void InitializeTitle()override;

	/// <summary>
	/// タイトルシーンUI更新
	/// </summary>
	void UpdateTitle()override;

	/// <summary>
	/// タイトルシーンUI描画
	/// </summary>
	void DrawTitle()override;

	/// <summary>
	/// UI初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// UI更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// UI描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// リザルトシーンUI初期化
	/// </summary>
	void InitializeResult()override;

	/// <summary>
	/// リザルトシーンUI更新
	/// </summary>
	void UpdateResult()override;

	/// <summary>
	/// リザルトシーンUI描画
	/// </summary>
	void DrawResult()override;

	/// <summary>
	/// 完全に見つかったかどうか取得
	/// </summary>
	/// <returns></returns>
	bool GetIsFound()const { return m_isFound; }

private:
	ComPtr<ID3D11ShaderResourceView> m_baseTexture;		//デフォルトのテクスチャ
	ComPtr<ID3D11ShaderResourceView> m_foundTexture;	//見つかり判定結果表示テクスチャ

	ComPtr<ID3D11Resource> m_baseTextureResource;		//デフォルトのテクスチャ情報
	ComPtr<ID3D11Resource> m_foundTextureResource;		//見つかり判定結果表示テクスチャ情報

	Vector2 m_baseTexturePos;		//デフォルトテクスチャ座標
	Vector2 m_foundTexturePos;		//見つかり判定テクスチャ座標

	RECT m_rectangle;		//テクスチャの表示範囲

	float m_baseTextureRotation;	//デフォルトテクスチャ回転量
	float m_foundTextureRotation;	//見つかり判定テクスチャ回転量

	Vector2 m_baseTextureOrigin;	//デフォルトテクスチャ中心座標
	Vector2 m_foundTextureOrigin;	//見つかり判定テクスチャ中心座標

	Vector2 m_baseTextureScale;		//デフォルトテクスチャスケール
	Vector2 m_foundTextureScale;	//見つかり判定テクスチャスケール

	float m_baseTextureDepth;		//デフォルトテクスチャ深度
	float m_foundTextureDepth;		//見つかり判定テクスチャ深度
		
	float m_foundTime;				//見つかり続けている時間
	const float m_maxFoundTime;		//発見までの時間

	XMVECTOR m_color;		//テクスチャのカラー
	float m_textureHeight;	//テクスチャの高さ

	bool m_isFound;			//完全に見つかったかどうか
};

