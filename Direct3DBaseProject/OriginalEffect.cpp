#include "pch.h"
#include"Json.h"
#include "OriginalEffect.h"
#include"ReadData.h"

//エフェクトの初期化
OriginalEffect::OriginalEffect(ID3D11Device* device, PixelType type, bool isSkinning):
	m_type(type),
	m_matrixBuffer(device),
	m_skinnedBuffer(device),
	m_lightBuffer(device),
	m_LVPBuffer(device),
	m_skinnedConstants(),
	m_light()
{
	assert((sizeof(OriginalEffect::MatrixConstants) % 16) == 0, "CB size alignment");
	assert((sizeof(OriginalEffect::SkinnedConstants) % 16) == 0, "CB size alignment");
	assert((sizeof(OriginalEffect::LightConstants) % 16) == 0, "CB size alignment");

	//頂点シェーダーのロード
	if (isSkinning)m_vsBlob = DX::ReadData(L"SkinningVertex.cso");
	else if(m_type==PixelType::Block) m_vsBlob = DX::ReadData(L"BlockVertex.cso");
	else m_vsBlob = DX::ReadData(L"VertexShader.cso");
	DX::ThrowIfFailed(device->CreateVertexShader(m_vsBlob.data(), m_vsBlob.size(), nullptr, m_vs.ReleaseAndGetAddressOf()));

	//ピクセルシェーダーのロード
	vector<uint8_t> psBlob;
	switch (m_type)
	{
	case PixelType::Object:
		psBlob = DX::ReadData(L"Lighting.cso");
		break;
	case PixelType::Character:
		psBlob = DX::ReadData(L"CharacterLighting.cso");
		break;
	case PixelType::Block:
		psBlob = DX::ReadData(L"BlockPixel.cso");
		break;
	default:
		break;
	}
	DX::ThrowIfFailed(device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_ps.ReleaseAndGetAddressOf()));

	//シャドウマップ描画用ピクセルシェーダーのロード
	
	psBlob = DX::ReadData(L"ObjectShadow.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_objectShadow.ReleaseAndGetAddressOf()));
	psBlob = DX::ReadData(L"BlockShadow.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_blockShadow.ReleaseAndGetAddressOf()));
	psBlob = DX::ReadData(L"Red.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_red.ReleaseAndGetAddressOf()));
	psBlob = DX::ReadData(L"Blue.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_blue.ReleaseAndGetAddressOf()));

	m_light.range = float(Json::GetInstance()->GetData()["LightRange"]);
	m_light.angle = XMConvertToRadians(float(Json::GetInstance()->GetData()["LightAngle"]));
}

//エフェクトの適用
void OriginalEffect::Apply(ID3D11DeviceContext* context)
{
	//定数バッファの設定
	MatrixConstants matrixConstants;
	matrixConstants.world = m_world;
	matrixConstants.view = m_view;
	matrixConstants.projection = m_projection;

	m_matrixBuffer.SetData(context, matrixConstants);

	m_skinnedBuffer.SetData(context, m_skinnedConstants);
	m_lightBuffer.SetData(context, m_light);

	LVPConstants LVP;
	LVP.LVP = XMMatrixMultiply(m_lightView, m_projection);
	m_LVPBuffer.SetData(context, LVP);

	//定数バッファとSRVのセット
	auto mb = m_matrixBuffer.GetBuffer();
	auto sb = m_skinnedBuffer.GetBuffer();
	auto lb = m_lightBuffer.GetBuffer();
	auto lvpb = m_LVPBuffer.GetBuffer();
	context->VSSetConstantBuffers(0, 1, &mb);
	context->VSSetConstantBuffers(1, 1, &sb);
	context->VSSetConstantBuffers(3, 1, &lvpb);
	context->PSSetConstantBuffers(2, 1, &lb);
	context->PSSetShaderResources(0, 1, m_texture1.GetAddressOf());
	if (m_type == PixelType::Block)
	{
		context->PSSetShaderResources(1, 1, m_texture2.GetAddressOf());
		context->PSSetShaderResources(2, 1, m_texture3.GetAddressOf());
		context->PSSetShaderResources(3, 1, m_texture4.GetAddressOf());
	}
	else
	{
		context->PSSetShaderResources(1, 1, m_normal.GetAddressOf());
		context->PSSetShaderResources(2, 1, m_ao.GetAddressOf());
	}

	//シェーダーの適用
	context->VSSetShader(m_vs.Get(), nullptr, 0);
	switch (m_type)
	{
	case PixelType::Shadow:
		context->PSSetShader(m_objectShadow.Get(), nullptr, 0);
		break;
	case PixelType::BlockShadow:
		context->PSSetShader(m_blockShadow.Get(), nullptr, 0);
		break;
	case PixelType::Red:
		context->PSSetShader(m_red.Get(), nullptr, 0);
		break;
	case PixelType::Blue:
		context->PSSetShader(m_blue.Get(), nullptr, 0);
		break;
	default:
		context->PSSetShader(m_ps.Get(), nullptr, 0);
		break;
	}
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
	m_texture1 = value;
}

//ブロックのテクスチャ設定
void OriginalEffect::SetBlockTexture(ID3D11ShaderResourceView* value1, ID3D11ShaderResourceView* value2, ID3D11ShaderResourceView* value3, ID3D11ShaderResourceView* value4)
{
	m_texture1 = value1;
	m_texture2 = value2;
	m_texture3 = value3;
	m_texture4 = value4;
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
}

//ビュー行列の設定
void OriginalEffect::SetView(FXMMATRIX view)
{
	m_view = view;
}

//プロジェクション行列の設定
void OriginalEffect::SetProjection(FXMMATRIX projection)
{
	m_projection = projection;
}

//ワールド,ビュー,プロジェクション行列の設定
void OriginalEffect::SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)
{
	m_world = world;
	m_view = view;
	m_projection = projection;
}

//影響を受けるボーン数の設定(処理なし)
void OriginalEffect::SetWeightsPerVertex(int value)
{
	
}

//ボーン変換行列の設定
void OriginalEffect::SetBoneTransforms(_In_reads_(count) XMMATRIX const* value, size_t count)
{
	if(count>MaxBones)throw std::invalid_argument("count parameter exceeds MaxBones");
	auto boneConstant = m_skinnedConstants.bones;
	for (size_t i = 0; i < count; i++)
	{
		XMStoreFloat3x4A(reinterpret_cast<XMFLOAT3X4A*>(&boneConstant[i]), value[i]);
	}
}

//ボーン変換行列のリセット
void OriginalEffect::ResetBoneTransforms()
{
	for (size_t i = 0; i < MaxBones; ++i)
	{
		m_skinnedConstants.bones[i][0] = g_XMIdentityR0;
		m_skinnedConstants.bones[i][1] = g_XMIdentityR1;
		m_skinnedConstants.bones[i][2] = g_XMIdentityR2;
	}
}

//スポットライトの座標設定
void OriginalEffect::SetLightPosition(Vector3 position)
{
	m_light.position = position;
}

//スポットライトの向き設定
void OriginalEffect::SetLightDirection(Vector3 direction)
{
	m_light.direction = direction;
}

//目の座標設定
void OriginalEffect::SetEyePosition(Vector3 eyePosition)
{
	m_light.eyePosition = eyePosition;
}

//ライトの空間行列設定
void OriginalEffect::SetLightView(Matrix view)
{
	m_lightView = view;
}