#pragma once
#include"UIBase.h"
using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

class UIBase;

/// <summary>
/// Aボタン管理クラス
/// </summary>
class ButtonA:public UIBase
{
public:
	/// <summary>
	/// AボタンUIの初期化
	/// </summary>
	ButtonA();

	/// <summary>
	/// データ破棄
	/// </summary>
	~ButtonA();

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
	/// テクスチャのスケールの更新
	/// </summary>
	void UpdateScale();

private:
	ComPtr<ID3D11ShaderResourceView> m_texture;		//テクスチャ
	ComPtr<ID3D11Resource> m_textureResource;		//テクスチャ情報

	Vector2 m_pos;		//テクスチャの座標
	Vector2 m_origin;	//テクスチャの中心座標
	float m_alpha;		//テクスチャのアルファ値
	float m_scale;		//テクスチャのスケール
	float m_scaleAdd;	//テクスチャのスケール拡縮量

	const float m_layerDepth;	//テクスチャのレイヤー深度
	const float m_minScale;		//テクスチャの最小スケール
	const float m_maxScale;		//テクスチャの最大スケール
};

