#include "pch.h"
#include"Json.h"
#include "OriginalEffect.h"
#include"ReadData.h"

//�G�t�F�N�g�̏�����
OriginalEffect::OriginalEffect(ID3D11Device* device, PixelType type, bool isSkinning):
	m_type(type),
	m_matrixBuffer(device),
	m_skinnedBuffer(device),
	m_lightBuffer(device),
	m_skinnedConstants(),
	m_light()
{
	assert((sizeof(OriginalEffect::MatrixConstants) % 16) == 0, "CB size alignment");
	assert((sizeof(OriginalEffect::SkinnedConstants) % 16) == 0, "CB size alignment");
	assert((sizeof(OriginalEffect::LightConstants) % 16) == 0, "CB size alignment");

	//���_�V�F�[�_�[�̃��[�h
	if (isSkinning)m_vsBlob = DX::ReadData(L"SkinningVertex.cso");
	else m_vsBlob = DX::ReadData(L"VertexShader.cso");
	DX::ThrowIfFailed(device->CreateVertexShader(m_vsBlob.data(), m_vsBlob.size(), nullptr, m_vs.ReleaseAndGetAddressOf()));

	//�s�N�Z���V�F�[�_�[�̃��[�h
	vector<uint8_t> psBlob;
	switch (m_type)
	{
	case PixelType::Object:
		psBlob = DX::ReadData(L"Lighting.cso");
		break;
	case PixelType::Character:
		psBlob = DX::ReadData(L"CharacterLighting.cso");
		break;
	}
	DX::ThrowIfFailed(device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_ps.ReleaseAndGetAddressOf()));

	m_light.range = float(Json::GetInstance()->GetData()["LightRange"]);
	m_light.angle = XMConvertToRadians(float(Json::GetInstance()->GetData()["LightAngle"]));
}

//�G�t�F�N�g�̓K�p
void OriginalEffect::Apply(ID3D11DeviceContext* context)
{
	//�萔�o�b�t�@�̐ݒ�
	MatrixConstants matrixConstants;
	matrixConstants.world = m_world;
	matrixConstants.view = m_view;
	matrixConstants.projection = m_projection;

	XMMATRIX worldTranspose = XMMatrixTranspose(m_world);
	XMMATRIX worldInverseTransPose = XMMatrixInverse(nullptr, worldTranspose);
	matrixConstants.worldInverse[0] = worldInverseTransPose.r[0];
	matrixConstants.worldInverse[1] = worldInverseTransPose.r[1];
	matrixConstants.worldInverse[2] = worldInverseTransPose.r[2];

	m_matrixBuffer.SetData(context, matrixConstants);

	m_skinnedBuffer.SetData(context, m_skinnedConstants);
	m_lightBuffer.SetData(context, m_light);

	//�萔�o�b�t�@��SRV�̃Z�b�g
	auto mb = m_matrixBuffer.GetBuffer();
	auto sb = m_skinnedBuffer.GetBuffer();
	auto lb = m_lightBuffer.GetBuffer();
	context->VSSetConstantBuffers(0, 1, &mb);
	context->VSSetConstantBuffers(1, 1, &sb);
	context->PSSetConstantBuffers(2, 1, &lb);
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
}

//�r���[�s��̐ݒ�
void OriginalEffect::SetView(FXMMATRIX view)
{
	m_view = view;
}

//�v���W�F�N�V�����s��̐ݒ�
void OriginalEffect::SetProjection(FXMMATRIX projection)
{
	m_projection = projection;
}

//���[���h,�r���[,�v���W�F�N�V�����s��̐ݒ�
void OriginalEffect::SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)
{
	m_world = world;
	m_view = view;
	m_projection = projection;
}

//�e�����󂯂�{�[�����̐ݒ�(�����Ȃ�)
void OriginalEffect::SetWeightsPerVertex(int value)
{
	
}

//�{�[���ϊ��s��̐ݒ�
void OriginalEffect::SetBoneTransforms(_In_reads_(count) XMMATRIX const* value, size_t count)
{
	if(count>MaxBones)throw std::invalid_argument("count parameter exceeds MaxBones");
	auto boneConstant = m_skinnedConstants.bones;
	for (size_t i = 0; i < count; i++)
	{
		XMStoreFloat3x4A(reinterpret_cast<XMFLOAT3X4A*>(&boneConstant[i]), value[i]);
	}
}

//�{�[���ϊ��s��̃��Z�b�g
void OriginalEffect::ResetBoneTransforms()
{
	for (size_t i = 0; i < MaxBones; ++i)
	{
		m_skinnedConstants.bones[i][0] = g_XMIdentityR0;
		m_skinnedConstants.bones[i][1] = g_XMIdentityR1;
		m_skinnedConstants.bones[i][2] = g_XMIdentityR2;
	}
}

void OriginalEffect::SetLightPosition(Vector3 position)
{
	m_light.position = position;
}

void OriginalEffect::SetLightDirection(Vector3 direction)
{
	m_light.direction = direction;
}

void OriginalEffect::SetEyePosition(Vector3 eyePosition)
{
	m_light.eyePosition = eyePosition;
}
