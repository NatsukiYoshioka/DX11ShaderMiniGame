#pragma once

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

class UIBase;

class GameOverUI:public UIBase
{
public:
	/// <summary>
	/// ゲームオーバー時UIの初期化
	/// </summary>
	GameOverUI();

	/// <summary>
	/// データ破棄
	/// </summary>
	~GameOverUI();

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

private:
	ComPtr<ID3D11ShaderResourceView> m_texture;		//テクスチャ
	ComPtr<ID3D11Resource> m_textureResource;		//テクスチャ情報

	Vector2 m_pos;		//テクスチャ座標
	Vector2 m_origin;	//テクスチャ中心座標
	float m_alpha;		//テクスチャのアルファ値

	const float m_scale;		//テクスチャのスケール
	const float m_layerDepth;	//テクスチャのレイヤー深度
};

