#include "pch.h"
#include "OriginalEffect.h"
#include"ReadData.h"

namespace
{
	constexpr uint32_t DirtyConstantBuffer = 0x1;
	constexpr uint32_t DirtyWVPMatrix = 0x2;
}

//エフェクトの初期化
OriginalEffect::OriginalEffect(ID3D11Device* device):
	m_dirtyFlags(uint32_t(-1)),
	m_matrixBuffer(device)
{
	assert((sizeof(OriginalEffect::MatrixConstants) % 16) == 0, "CB size alignment");

	//頂点シェーダーのロード
	m_vsBlob = DX::ReadData(L"VertexShader.cso");
	DX::ThrowIfFailed(device->CreateVertexShader(m_vsBlob.data(), m_vsBlob.size(), nullptr, m_vs.ReleaseAndGetAddressOf()));

	//ピクセルシェーダーのロード
	vector<uint8_t> psBlob;
	psBlob = DX::ReadData(L"TestPixel.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_ps.ReleaseAndGetAddressOf()));
}

//エフェクトの適用
void OriginalEffect::Apply(ID3D11DeviceContext* context)
{
	if (m_dirtyFlags & DirtyConstantBuffer)
	{
		//定数バッファの設定
		MatrixConstants matrixConstants;
		matrixConstants.world = m_world;
		matrixConstants.view = m_view;
		matrixConstants.projection = m_projection;
		m_matrixBuffer.SetData(context, matrixConstants);

		m_dirtyFlags &= ~DirtyConstantBuffer;
	}

	//定数バッファとSRVのセット
	auto mb = m_matrixBuffer.GetBuffer();
	context->VSSetConstantBuffers(0, 1, &mb);
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	context->PSSetShaderResources(1, 1, m_normal.GetAddressOf());
	context->PSSetShaderResources(2, 1, m_ao.GetAddressOf());

	//シェーダーの適用
	context->VSSetShader(m_vs.Get(), nullptr, 0);
	context->PSSetShader(m_ps.Get(), nullptr, 0);
}

//頂点シェーダーのバイトコード取得
void OriginalEffect::GetVertexShaderBytecode(void const** pShaderBytecode, size_t* pBytecodeLength)
{
	assert(pShaderBytecode != nullptr && pBytecodeLength != nullptr);
	*pShaderBytecode = m_vsBlob.data();
	*pBytecodeLength = m_vsBlob.size();
}

//テクスチャの設定
void OriginalEffect::SetTexture(ID3D11ShaderResourceView* value)
{
	m_texture = value;
}

//法線マップの設定
void OriginalEffect::SetNormal(ID3D11ShaderResourceView* value)
{
	m_normal = value;
}

//AOマップの設定
void OriginalEffect::SetAO(ID3D11ShaderResourceView* value)
{
	m_ao = value;
}

//ワールド行列の設定
void OriginalEffect::SetWorld(FXMMATRIX world)
{
	m_world = world;
	m_dirtyFlags |= DirtyWVPMatrix;
}

//ビュー行列の設定
void OriginalEffect::SetView(FXMMATRIX view)
{
	m_view = view;
	m_dirtyFlags |= DirtyWVPMatrix;
}

//プロジェクション行列の設定
void OriginalEffect::SetProjection(FXMMATRIX projection)
{
	m_projection = projection;
	m_dirtyFlags |= DirtyWVPMatrix;
}

//ワールド,ビュー,ロジェクション行列の設定
void OriginalEffect::SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)
{
	m_world = world;
	m_view = view;
	m_projection = projection;
	m_dirtyFlags |= DirtyWVPMatrix;
}