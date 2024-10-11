#pragma once
#include"UIBase.h"

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
	/// �^�C�g���V�[��UI������
	/// </summary>
	void InitializeTitle()override;

	/// <summary>
	/// �^�C�g���V�[��UI�X�V
	/// </summary>
	void UpdateTitle()override;

	/// <summary>
	/// �^�C�g���V�[��UI�`��
	/// </summary>
	void DrawTitle()override;

	/// <summary>
	/// UI������
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// UI�X�V
	/// </summary>
	void Update()override;

	/// <summary>
	/// UI�`��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// ���U���g�V�[��UI������
	/// </summary>
	void InitializeResult()override;

	/// <summary>
	/// ���U���g�V�[��UI�X�V
	/// </summary>
	void UpdateResult()override;

	/// <summary>
	/// ���U���g�V�[��UI�`��
	/// </summary>
	void DrawResult()override;

	float GetAlpha()const { return m_alpha; }

	bool GetIsFinishFadeout()const { return m_isFinishFadeout; }

	bool GetIsFinishFadein()const { return m_isFinishFadein; }

private:
	ComPtr<ID3D11ShaderResourceView> m_texture;
	ComPtr<ID3D11Resource> m_textureResource;

	Vector2 m_origin;
	float m_alpha;

	bool m_isFinishFadeout;
	bool m_isFinishFadein;

	const float m_scale;
	const float m_layerDepth;
	const float m_alphaAdd;
};

