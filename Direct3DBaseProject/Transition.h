#pragma once
#include"UIBase.h"

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

class UIBase;

class Transition :public UIBase
{
public:
	/// <summary>
	/// トランジションの初期化
	/// </summary>
	Transition();

	/// <summary>
	/// データ破棄
	/// </summary>
	~Transition();

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
	/// テクスチャのアルファ値取得
	/// </summary>
	/// <returns></returns>
	float GetAlpha()const { return m_alpha; }

	/// <summary>
	/// トランジションのフェードアウトが終わったかどうか取得
	/// </summary>
	/// <returns></returns>
	bool GetIsFinishFadeout()const { return m_isFinishFadeout; }

	/// <summary>
	/// トランジションのフェードインが終わったかどうか取得
	/// </summary>
	/// <returns></returns>
	bool GetIsFinishFadein()const { return m_isFinishFadein; }

private:
	ComPtr<ID3D11ShaderResourceView> m_texture;		//テクスチャ
	ComPtr<ID3D11Resource> m_textureResource;		//テクスチャ情報

	Vector2 m_origin;		//テクスチャ中心座標
	float m_alpha;			//テクスチャのアルファ値

	bool m_isFinishFadeout;	//フェードアウトが終わったかどうか
	bool m_isFinishFadein;	//フェードインが終わったかどうか

	const float m_scale;		//テクスチャのスケール
	const float m_layerDepth;	//テクスチャのレイヤー深度
	const float m_alphaAdd;		//アルファ値の増減量
};

