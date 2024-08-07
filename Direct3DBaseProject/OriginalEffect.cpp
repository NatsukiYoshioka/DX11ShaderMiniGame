#include "pch.h"
#include "OriginalEffect.h"
#include"ReadData.h"

namespace
{
	constexpr uint32_t DirtyConstantBuffer = 0x1;
	constexpr uint32_t DirtyWVPMatrix = 0x2;
}

//�G�t�F�N�g�̏�����
OriginalEffect::OriginalEffect(ID3D11Device* device):
	m_dirtyFlags(uint32_t(-1)),
	m_matrixBuffer(device)
{
	assert((sizeof(OriginalEffect::MatrixConstants) % 16) == 0, "CB size alignment");

	//���_�V�F�[�_�[�̃��[�h
	m_vsBlob = DX::ReadData(L"VertexShader.cso");
	DX::ThrowIfFailed(device->CreateVertexShader(m_vsBlob.data(), m_vsBlob.size(), nullptr, m_vs.ReleaseAndGetAddressOf()));

	//�s�N�Z���V�F�[�_�[�̃��[�h
	vector<uint8_t> psBlob;
	psBlob = DX::ReadData(L"TestPixel.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_ps.ReleaseAndGetAddressOf()));
}

//�G�t�F�N�g�̓K�p
void OriginalEffect::Apply(ID3D11DeviceContext* context)
{
	if (m_dirtyFlags & DirtyConstantBuffer)
	{
		//�萔�o�b�t�@�̐ݒ�
		MatrixConstants matrixConstants;
		matrixConstants.world = m_world;
		matrixConstants.view = m_view;
		matrixConstants.projection = m_projection;
		m_matrixBuffer.SetData(context, matrixConstants);

		m_dirtyFlags &= ~DirtyConstantBuffer;
	}

	//�萔�o�b�t�@��SRV�̃Z�b�g
	auto mb = m_matrixBuffer.GetBuffer();
	context->VSSetConstantBuffers(0, 1, &mb);
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	context->PSSetShaderResources(1, 1, m_normal.GetAddressOf());
	context->PSSetShaderResources(2, 1, m_ao.GetAddressOf());

	//�V�F�[�_�[�̓K�p
	context->VSSetShader(m_vs.Get(), nullptr, 0);
	context->PSSetShader(m_ps.Get(), nullptr, 0);
}

//���_�V�F�[�_�[�̃o�C�g�R�[�h�擾
void OriginalEffect::GetVertexShaderBytecode(void const** pShaderBytecode, size_t* pBytecodeLength)
{
	assert(pShaderBytecode != nullptr && pBytecodeLength != nullptr);
	*pShaderBytecode = m_vsBlob.data();
	*pBytecodeLength = m_vsBlob.size();
}

//�e�N�X�`���̐ݒ�
void OriginalEffect::SetTexture(ID3D11ShaderResourceView* value)
{
	m_texture = value;
}

//�@���}�b�v�̐ݒ�
void OriginalEffect::SetNormal(ID3D11ShaderResourceView* value)
{
	m_normal = value;
}

//AO�}�b�v�̐ݒ�
void OriginalEffect::SetAO(ID3D11ShaderResourceView* value)
{
	m_ao = value;
}

//���[���h�s��̐ݒ�
void OriginalEffect::SetWorld(FXMMATRIX world)
{
	m_world = world;
	m_dirtyFlags |= DirtyWVPMatrix;
}

//�r���[�s��̐ݒ�
void OriginalEffect::SetView(FXMMATRIX view)
{
	m_view = view;
	m_dirtyFlags |= DirtyWVPMatrix;
}

//�v���W�F�N�V�����s��̐ݒ�
void OriginalEffect::SetProjection(FXMMATRIX projection)
{
	m_projection = projection;
	m_dirtyFlags |= DirtyWVPMatrix;
}

//���[���h,�r���[,���W�F�N�V�����s��̐ݒ�
void OriginalEffect::SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)
{
	m_world = world;
	m_view = view;
	m_projection = projection;
	m_dirtyFlags |= DirtyWVPMatrix;
}