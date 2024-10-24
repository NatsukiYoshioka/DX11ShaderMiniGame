#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"Enemy.h"
#include"EnemyAccessor.h"
#include"GameObject.h"
#include"OriginalEffect.h"
#include"FoundUI.h"
#include"UIBase.h"
#include"UIAccessor.h"
#include<WinBase.h>
#include<random>
#define _USE_MATH_DEFINES
#include<math.h>
#include"GameObjectManager.h"
#include "Block.h"

//�ςݖ؃u���b�N�̏�����
Block::Block(Model* modelHandle, vector<Vector3> pos, vector<float> rotate, vector<int> textureID):
	m_rotate()
{
	m_model = modelHandle;

	//�C���X�^���V���O�p�̃��[���h���W�ƃe�N�X�`��ID�̐ݒ�
	for (int i = 0; i < pos.size(); i++)
	{
		InstanceData instance;
		instance.world = Matrix::Identity;
		instance.world = XMMatrixMultiply(instance.world, Matrix::CreateScale(0.0125f));
		instance.world = XMMatrixMultiply(instance.world, Matrix::CreateRotationY(rotate.at(i) * XM_PI / 180.f));
		instance.world = XMMatrixMultiply(instance.world, XMMatrixTranslation(pos.at(i).x, pos.at(i).y, pos.at(i).z));
		instance.textureID = textureID.at(i);
		m_instances.push_back(instance);
	}

	auto deviceAccessor = DeviceAccessor::GetInstance();
	auto device = deviceAccessor->GetDevice();

	// �C���X�^���X�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC instanceBufferDesc = {};
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceData) * m_instances.size();
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA instanceData = {};
	instanceData.pSysMem = m_instances.data();
	device->CreateBuffer(&instanceBufferDesc, &instanceData, m_instanceBuffer.ReleaseAndGetAddressOf());

	//�G�t�F�N�g�̏�����
	SetCurrentDirectory(L"Assets/Shader");
	m_effect = make_unique<OriginalEffect>(device, OriginalEffect::PixelType::Block);
	SetCurrentDirectory(L"../../");

	//���̓��C�A�E�g�̒�`�̐ݒ�
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		// ���_�f�[�^
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// �C���X�^���X�f�[�^
		{ "INSTANCE_TRANSFORM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE_TRANSFORM", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE_TRANSFORM", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE_TRANSFORM", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32_UINT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};

	//���̓��C�A�E�g�̍쐬
	const void* shaderByteCode;
	size_t byteCodeLength;
	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(layout, ARRAYSIZE(layout), shaderByteCode, byteCodeLength, m_inputLayout.ReleaseAndGetAddressOf());

	//�e�N�X�`���̃��[�h�Ɛݒ�
	auto json = Json::GetInstance();
	ComPtr<ID3D11ShaderResourceView> texture1;
	ComPtr<ID3D11ShaderResourceView> texture2;
	ComPtr<ID3D11ShaderResourceView> texture3;
	ComPtr<ID3D11ShaderResourceView> texture4;
	SetCurrentDirectory(L"Assets/Models");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device,
		json->Widen(json->GetData()["BlockTexture"].at(0)).c_str(),
		nullptr,
		texture1.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device,
		json->Widen(json->GetData()["BlockTexture"].at(1)).c_str(),
		nullptr,
		texture2.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device,
		json->Widen(json->GetData()["BlockTexture"].at(2)).c_str(),
		nullptr,
		texture3.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device,
		json->Widen(json->GetData()["BlockTexture"].at(3)).c_str(),
		nullptr,
		texture4.ReleaseAndGetAddressOf()));
	SetCurrentDirectory(L"../../");
	m_effect->SetBlockTexture(texture1.Get(), texture2.Get(), texture3.Get(), texture4.Get());

	VertexPositionNormalTexture* vertices;
	for (const auto& mit : m_model->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);
		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			//���_�f�[�^�̎擾
			D3D11_BUFFER_DESC vbDesc;
			part->vertexBuffer->GetDesc(&vbDesc);
			D3D11_BUFFER_DESC vbufferDesc = {};
			vbufferDesc.Usage = D3D11_USAGE_STAGING;
			vbufferDesc.ByteWidth = vbDesc.ByteWidth;
			vbufferDesc.BindFlags = 0;
			vbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			vbufferDesc.StructureByteStride = sizeof(VertexPositionNormalTexture);
			ID3D11Buffer* vBuffer;
			deviceAccessor->GetDevice()->CreateBuffer(&vbufferDesc, NULL, &vBuffer);
			deviceAccessor->GetContext()->CopyResource(vBuffer, part->vertexBuffer.Get());

			D3D11_MAPPED_SUBRESOURCE mapped;
			deviceAccessor->GetContext()->Map(vBuffer, 0, D3D11_MAP_READ, 0, &mapped);
			vertices = reinterpret_cast<VertexPositionNormalTexture*>(mapped.pData);
			deviceAccessor->GetContext()->Unmap(vBuffer, 0);

			//���_�f�[�^�����[���h���W�ϊ�
			for (size_t i = 0; i < vbufferDesc.ByteWidth / vbufferDesc.StructureByteStride; i++)
			{
				XMVECTOR vertexPosition = XMLoadFloat3(&vertices[i].position);
				for (int j = 0; j < m_instances.size(); j++)
				{
					XMVECTOR worldPosition = XMVector3Transform(vertexPosition, m_instances.at(j).world);
					XMFLOAT3 pos;
					XMStoreFloat3(&pos, worldPosition);
					m_vertices.push_back(pos);
				}
			}
		}
	}

	//���_�o�b�t�@SRV�쐬
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * m_vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	vertexBufferDesc.StructureByteStride = sizeof(DirectX::XMFLOAT3);

	D3D11_SUBRESOURCE_DATA vertexInitData = {};
	vertexInitData.pSysMem = m_vertices.data();

	ID3D11Buffer* vertexBuffer;
	device->CreateBuffer(&vertexBufferDesc, &vertexInitData, &vertexBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = m_vertices.size();
	device->CreateShaderResourceView(vertexBuffer, &srvDesc, m_vertexBufferSRV.ReleaseAndGetAddressOf());
}

//�f�[�^�j��
Block::~Block()
{
	m_modelHandle.reset();
}

//�^�C�g���V�[���I�u�W�F�N�g�̏�����
void Block::InitializeTitle()
{

}

//�^�C�g���V�[���I�u�W�F�N�g�̍X�V
void Block::UpdateTitle()
{
	
}

//�^�C�g���V�[���I�u�W�F�N�g�̕`��
void Block::DrawTitle()
{

}

//�I�u�W�F�N�g�̏�����
void Block::Initialize()
{

}

//�I�u�W�F�N�g�̍X�V
void Block::Update()
{
	m_effect->SetWorld(Matrix::Identity);
	m_effect->SetView(CameraAccessor::GetInstance()->GetCamera()->GetView());
	m_effect->SetProjection(CameraAccessor::GetInstance()->GetCamera()->GetProjection());
	m_effect->SetLightPosition(EnemyAccessor::GetInstance()->GetEnemy()->GetEyePosition());
	m_effect->SetLightDirection(EnemyAccessor::GetInstance()->GetEnemy()->GetEyeDirection());
	m_effect->SetEyePosition(CameraAccessor::GetInstance()->GetCamera()->GetPos());
	for (int i = 0; i < UIAccessor::GetInstance()->GetUIs().size(); i++)
	{
		auto foundUI = dynamic_cast<FoundUI*>(UIAccessor::GetInstance()->GetUIs().at(i));
		if (foundUI)
		{
			//m_effect->SetLightColor(Vector3(1.f, 1.f - foundUI->GetTimeRatio(), 1.f - foundUI->GetTimeRatio()));
			m_effect->SetLightColor(Vector3(1.f, 1.f, 1.f));
			break;
		}
	}
	m_effect->SetLightView(EnemyAccessor::GetInstance()->GetEnemy()->GetEyeView());
}

//�I�u�W�F�N�g�̕`��
void Block::Draw()
{
	auto states = DeviceAccessor::GetInstance()->GetStates();
	auto context = DeviceAccessor::GetInstance()->GetContext();

	UINT strides[2] = { sizeof(VertexPositionNormalTexture),sizeof(InstanceData) };
	UINT offsets[2] = { 0,0 };
	ID3D11Buffer* buffers[2];
	for (const auto& mit : m_model->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);
		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			buffers[0] = part->vertexBuffer.Get();
			buffers[1] = m_instanceBuffer.Get();

			context->IASetVertexBuffers(0, 2, buffers, strides, offsets);
			context->IASetIndexBuffer(part->indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
			context->OMSetDepthStencilState(states->DepthDefault(), 0);
			context->RSSetState(states->CullClockwise());
			ID3D11SamplerState* samplers[] =
			{
				states->LinearWrap(),
				states->LinearWrap(),
			};
			context->PSSetSamplers(0, 2, samplers);

			m_effect->Apply(context);
			context->IASetInputLayout(m_inputLayout.Get());
			context->DrawIndexedInstanced(part->indexCount, m_instances.size(), 0, 0, 0);
		}
	}
	
	/*m_modelHandle->Draw(DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		m_world,
		CameraAccessor::GetInstance()->GetCamera()->GetView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection());*/
	//m_primitiveBatch->DrawIndexed()
}

//���U���g�V�[���I�u�W�F�N�g�̏�����
void Block::InitializeResult()
{

}

//���U���g�V�[���I�u�W�F�N�g�̍X�V
void Block::UpdateResult()
{
	
}

//���U���g�V�[���I�u�W�F�N�g�̕`��
void Block::DrawResult()
{
	
}

//�e�̕`��
void Block::DrawShadow()
{
	//�V�F�[�_�[���e�p�ɕύX
	m_effect->UpdateType(OriginalEffect::PixelType::BlockShadow);
	m_effect->SetView(EnemyAccessor::GetInstance()->GetEnemy()->GetEyeView());

	//�`��
	Draw();

	//�V�F�[�_�[�����ɖ߂�
	m_effect->UpdateType(OriginalEffect::PixelType::Block);
	m_effect->SetView(CameraAccessor::GetInstance()->GetCamera()->GetView());
}

//�G���_�ł̕`��
void Block::DrawHitCheck()
{
	m_effect->UpdateType(OriginalEffect::PixelType::Red);
	m_effect->SetView(EnemyAccessor::GetInstance()->GetEnemy()->GetEyeView());

	Draw();

	m_effect->UpdateType(OriginalEffect::PixelType::Block);
	m_effect->SetView(CameraAccessor::GetInstance()->GetCamera()->GetView());
}