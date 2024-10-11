#pragma once
#include"UIBase.h"
using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

class UIBase;

class ButtonA:public UIBase
{
public:
	ButtonA();
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

	void UpdateScale();

private:
	ComPtr<ID3D11ShaderResourceView> m_texture;
	ComPtr<ID3D11Resource> m_textureResource;

	Vector2 m_pos;
	Vector2 m_origin;
	float m_alpha;
	float m_scale;
	float m_scaleAdd;

	const float m_layerDepth;
	const float m_minScale;
	const float m_maxScale;
};

