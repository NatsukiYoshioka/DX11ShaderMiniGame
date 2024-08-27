#pragma once

using namespace Microsoft::WRL;

class UIBase;

class FoundUI:public UIBase
{
public:
	FoundUI();
	~FoundUI();

	void Update()override;
	void Draw()override;

private:
	ComPtr<ID3D11ShaderResourceView> m_baseTexture;
	ComPtr<ID3D11ShaderResourceView> m_foundTexture;

	ComPtr<ID3D11Resource> m_baseTextureResource;
	ComPtr<ID3D11Resource> m_foundTextureResource;

	Vector2 m_baseTexturePos;
	Vector2 m_foundTexturePos;

	float m_baseTextureRotation;
	float m_foundTextureRotation;

	Vector2 m_baseTextureOrigin;
	Vector2 m_foundTextureOrigin;

	Vector2 m_baseTextureScale;
	Vector2 m_foundTextureScale;

	float m_baseTextureDepth;
	float m_foundTextureDepth;
};

