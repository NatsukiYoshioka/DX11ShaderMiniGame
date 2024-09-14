#pragma once

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

class UIBase;

class Transition :public UIBase
{
public:
	Transition();
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

	bool GetIsFinishFadeout()const { return m_isFinishFadeout; }

	bool GetIsFinishFadein()const { return m_isFinishFadein; }

private:
	ComPtr<ID3D11ShaderResourceView> m_texture;
	ComPtr<ID3D11Resource> m_textureResource;

	Vector2 m_origin;
	float m_alpha;

	unique_ptr<BasicEffect> m_effect;
	ComPtr<ID3D11InputLayout> m_inputLayout;

	bool m_isFinishFadeout;
	bool m_isFinishFadein;

	const float m_scale;
	const float m_layerDepth;
	const float m_alphaAdd;
};

