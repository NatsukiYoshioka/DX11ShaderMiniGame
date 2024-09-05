#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"Enemy.h"
#include"EnemyAccessor.h"
#include"Block.h"
#include"BlockAccessor.h"
#include"OriginalEffect.h"
#include"GameObject.h"
#include"ReadData.h"
#include "Player.h"

extern void ExitGame() noexcept;

Player::Player(const wchar_t* fileName, Vector3 pos, float rotate):
	m_nowAnimationState(AnimationState::Idle),
	m_beFound(false),
	m_scale(float(Json::GetInstance()->GetData()["PlayerScale"])),
	m_speed(float(Json::GetInstance()->GetData()["PlayerSpeed"])),
	m_runSpeed(float(Json::GetInstance()->GetData()["PlayerRunSpeed"])),
	m_crouchSpeed(float(Json::GetInstance()->GetData()["PlayerCrouchSpeed"]))
{
	auto deviceAccessor = DeviceAccessor::GetInstance();

	//モデルのロード
	SetCurrentDirectory(L"Assets/Models");
	m_modelHandle = Model::CreateFromSDKMESH(deviceAccessor->GetDevice(),
		fileName,
		*deviceAccessor->GetEffectFactory(),
		ModelLoader_Clockwise | ModelLoader_IncludeBones);
	SetCurrentDirectory(L"../../");

	//シェーダーの初期化
	SetCurrentDirectory(L"Assets/Shader");
	m_effect = make_unique<OriginalEffect>(deviceAccessor->GetDevice(), OriginalEffect::PixelType::Character, true);
	vector<uint8_t> csBlob;
	csBlob = DX::ReadData(L"HitCalculation.cso");
	DX::ThrowIfFailed(deviceAccessor->GetDevice()->CreateComputeShader(csBlob.data(),
		csBlob.size(),
		nullptr,
		m_csForEnemyEye.ReleaseAndGetAddressOf()));
	csBlob = DX::ReadData(L"HitMeshSphereCalculation.cso");
	DX::ThrowIfFailed(deviceAccessor->GetDevice()->CreateComputeShader(csBlob.data(),
		csBlob.size(),
		nullptr,
		m_csForCollision.ReleaseAndGetAddressOf()));
	SetCurrentDirectory(L"../../");

	//モデルの各メッシュの描画設定
	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);

		
		if (mesh->boneIndex != ModelBone::c_Invalid)
		{
			for (const auto& pit : mesh->meshParts)
			{
				auto part = pit.get();
				assert(part != nullptr);

				//入力レイアウトの初期化とエフェクトの適用
				part->ModifyEffect(deviceAccessor->GetDevice(), m_effect, false);
			}
		}
	}
	m_modelHandle->Modified();

	//アニメーションの設定
	SetCurrentDirectory(L"Assets/Models");
	auto json = Json::GetInstance();
	m_animations.resize(json->GetData()["Player"].size() - 1);
	for (int i = 0; i < json->GetData()["Player"].size() - 1; i++)
	{
		DX::ThrowIfFailed(m_animations.at(i).Load(json->Widen(json->GetData()["Player"].at(i + 1)).c_str()));
		m_animations.at(i).Bind(*m_modelHandle);
	}
	SetCurrentDirectory(L"../../");

	auto nbones = m_modelHandle->bones.size();
	m_drawBones = ModelBone::MakeArray(nbones);

	//テクスチャのロード
	ComPtr<ID3D11ShaderResourceView> texture;
	SetCurrentDirectory(L"Assets/Models");
	DX::ThrowIfFailed(CreateDDSTextureFromFile(deviceAccessor->GetDevice(),
		json->Widen(json->GetData()["PlayerTexture"]).c_str(),
		nullptr,
		texture.ReleaseAndGetAddressOf()));
	SetCurrentDirectory(L"../../");

	//テクスチャの設定
	m_effect->SetTexture(texture.Get());

	//座標とY軸回転量の設定
	m_pos = pos;
	m_rotate = rotate * XM_PI / 180.f;

	//出力UAV設定
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.ByteWidth = sizeof(HitInfo);
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = sizeof(HitInfo);
	deviceAccessor->GetDevice()->CreateBuffer(&bufferDesc, NULL, m_bufferResult.ReleaseAndGetAddressOf());
	bufferDesc.ByteWidth = sizeof(Sphere);
	bufferDesc.StructureByteStride = sizeof(Sphere);
	deviceAccessor->GetDevice()->CreateBuffer(&bufferDesc, NULL, m_sphereResult.ReleaseAndGetAddressOf());

	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	ZeroMemory(&UAVDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.FirstElement = 0;
	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.Buffer.NumElements = 1;
	deviceAccessor->GetDevice()->CreateUnorderedAccessView(m_bufferResult.Get(), &UAVDesc, m_hitInfo.ReleaseAndGetAddressOf());
	deviceAccessor->GetDevice()->CreateUnorderedAccessView(m_sphereResult.Get(), &UAVDesc, m_sphereInfo.ReleaseAndGetAddressOf());
}

Player::~Player()
{
	m_modelHandle.reset();
}

void Player::Update()
{
	auto pad = DeviceAccessor::GetInstance()->GetGamePad()->GetState(0);

	bool isMove = false;
	bool isCrouch = false;
	float nowSpeed = 0.f;
	m_nowAnimationState = AnimationState::Idle;
	if (pad.IsConnected())
	{
		if (pad.IsViewPressed())
		{
			ExitGame();
		}
		if (pad.IsLeftShoulderPressed())
		{
			m_nowAnimationState = AnimationState::Crouch;
			isCrouch = true;
		}
		if (pad.thumbSticks.leftX != 0 || pad.thumbSticks.leftY != 0)
		{
			m_rotate = atan2f(-pad.thumbSticks.leftX, pad.thumbSticks.leftY);
			
			isMove = true;
			nowSpeed = m_speed;
			m_nowAnimationState = AnimationState::Walk;

			if (isCrouch)
			{
				nowSpeed = m_crouchSpeed;
				m_nowAnimationState = AnimationState::CrouchedWalk;
			}
			else if (pad.IsRightShoulderPressed())
			{
				nowSpeed = m_runSpeed;
				m_nowAnimationState = AnimationState::Run;
			}
		}
	}

	if (isMove)
	{
		auto cameraPitch = CameraAccessor::GetInstance()->GetCamera()->GetPitch();
		auto radian = cameraPitch * XM_PI / 180;
		m_rotate += radian;

		m_pos.z += -cos(m_rotate) * nowSpeed;
		m_pos.x += -sin(m_rotate) * nowSpeed;
	}

	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);
		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			auto effect = static_cast<OriginalEffect*>(part->effect.get());
			effect->SetLightPosition(EnemyAccessor::GetInstance()->GetEnemy()->GetEyePosition());
			effect->SetLightDirection(EnemyAccessor::GetInstance()->GetEnemy()->GetEyeDirection());
			effect->SetEyePosition(CameraAccessor::GetInstance()->GetCamera()->GetPos());
			effect->SetLightView(EnemyAccessor::GetInstance()->GetEnemy()->GetEyeView());
		}
	}

	HitCheckObject();

	//ワールド座標行列の更新
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(m_scale));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationY(m_rotate));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));

	m_animations.at(static_cast<int>(m_nowAnimationState)).Update(*DeviceAccessor::GetInstance()->GetElapsedTime());
}

void Player::Draw()
{
	size_t nbones = m_modelHandle->bones.size();

	m_animations.at(static_cast<int>(m_nowAnimationState)).Apply(*m_modelHandle, nbones, m_drawBones.get());

	m_modelHandle->DrawSkinned(DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		nbones,
		m_drawBones.get(),
		m_world,
		CameraAccessor::GetInstance()->GetCamera()->GetView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection());
}

void Player::DrawShadow()
{
	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);
		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			auto effect = static_cast<OriginalEffect*>(part->effect.get());
			effect->UpdateType(OriginalEffect::PixelType::Shadow);
		}
	}

	size_t nbones = m_modelHandle->bones.size();

	m_animations.at(static_cast<int>(m_nowAnimationState)).Apply(*m_modelHandle, nbones, m_drawBones.get());

	m_modelHandle->DrawSkinned(DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		nbones,
		m_drawBones.get(),
		m_world,
		EnemyAccessor::GetInstance()->GetEnemy()->GetEyeView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection());

	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);
		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			auto effect = static_cast<OriginalEffect*>(part->effect.get());
			effect->UpdateType(OriginalEffect::PixelType::Character);
		}
	}
}

void Player::DrawHitCheck()
{
	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);
		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			auto effect = static_cast<OriginalEffect*>(part->effect.get());
			effect->UpdateType(OriginalEffect::PixelType::Blue);
		}
	}

	size_t nbones = m_modelHandle->bones.size();

	m_animations.at(static_cast<int>(m_nowAnimationState)).Apply(*m_modelHandle, nbones, m_drawBones.get());

	m_modelHandle->DrawSkinned(DeviceAccessor::GetInstance()->GetContext(),
		*DeviceAccessor::GetInstance()->GetStates(),
		nbones,
		m_drawBones.get(),
		m_world,
		EnemyAccessor::GetInstance()->GetEnemy()->GetEyeView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection());

	for (const auto& mit : m_modelHandle->meshes)
	{
		auto mesh = mit.get();
		assert(mesh != nullptr);
		for (const auto& pit : mesh->meshParts)
		{
			auto part = pit.get();
			assert(part != nullptr);

			auto effect = static_cast<OriginalEffect*>(part->effect.get());
			effect->UpdateType(OriginalEffect::PixelType::Character);
		}
	}
}

void Player::HitCheck()
{
	auto deviceAccessor = DeviceAccessor::GetInstance();
	auto size = int(Json::GetInstance()->GetData()["HitCheckTextureSize"]);

	ID3D11Buffer* debugBuffer = NULL;
	D3D11_BUFFER_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_bufferResult->GetDesc(&BufferDesc);
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;  // CPU から読み込みできるように設定する
	BufferDesc.Usage = D3D11_USAGE_STAGING;             // GPU から CPU へのデータ転送 (コピー) をサポートするリソース
	BufferDesc.BindFlags = 0;
	BufferDesc.MiscFlags = 0;
	deviceAccessor->GetDevice()->CreateBuffer(&BufferDesc, NULL, &debugBuffer);
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceAccessor->GetContext()->Map(debugBuffer, 0, D3D11_MAP_WRITE, 0, &mappedResource);
	HitInfo* p = reinterpret_cast<HitInfo*>(mappedResource.pData);
	p->playerPixNum = 0;
	p->visiblePixNum = 0;
	deviceAccessor->GetContext()->Unmap(debugBuffer, 0);
	deviceAccessor->GetContext()->CopyResource(m_bufferResult.Get(), debugBuffer);
	
	deviceAccessor->GetContext()->CSSetUnorderedAccessViews(0, 1, m_hitInfo.GetAddressOf(), 0);
	deviceAccessor->GetContext()->CSSetShader(m_csForEnemyEye.Get(), nullptr, 0);
	deviceAccessor->GetContext()->Dispatch(size, size, 1);

	deviceAccessor->GetContext()->CSSetShader(nullptr, nullptr, 0);
	deviceAccessor->GetContext()->CSSetShaderResources(0, 0, nullptr);
	deviceAccessor->GetContext()->CSSetUnorderedAccessViews(0, 0, nullptr, nullptr);
	
	deviceAccessor->GetContext()->CopyResource(debugBuffer, m_bufferResult.Get());
	deviceAccessor->GetContext()->Map(debugBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
	p = reinterpret_cast<HitInfo*>(mappedResource.pData);
	deviceAccessor->GetContext()->Unmap(debugBuffer, 0);

	float visibleRatio = float(p->visiblePixNum) / p->playerPixNum;
	float maxVisibleRatio = 1.f / 4.f;
	if (visibleRatio > maxVisibleRatio)
	{
		m_beFound = true;
	}
	else m_beFound = false;
}

void Player::HitCheckObject()
{
	auto device = DeviceAccessor::GetInstance()->GetDevice();
	auto context = DeviceAccessor::GetInstance()->GetContext();
	auto blocks = BlockAccessor::GetInstance()->GetBlocks();

	for (int i = 0;i < blocks.size();i++)
	{
		ID3D11Buffer* debugBuffer = NULL;
		D3D11_BUFFER_DESC BufferDesc;
		ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_sphereResult->GetDesc(&BufferDesc);
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;  // CPU から読み込みできるように設定する
		BufferDesc.Usage = D3D11_USAGE_STAGING;             // GPU から CPU へのデータ転送 (コピー) をサポートするリソース
		BufferDesc.BindFlags = 0;
		BufferDesc.MiscFlags = 0;
		device->CreateBuffer(&BufferDesc, NULL, &debugBuffer);
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		context->Map(debugBuffer, 0, D3D11_MAP_WRITE, 0, &mappedResource);
		Sphere* p = reinterpret_cast<Sphere*>(mappedResource.pData);
		p->center = m_pos;
		p->radius = 0.3f;
		context->Unmap(debugBuffer, 0);
		context->CopyResource(m_sphereResult.Get(), debugBuffer);
		
		context->CSSetUnorderedAccessViews(0, 1, m_sphereInfo.GetAddressOf(), 0);
		context->CSSetShaderResources(0, 1, blocks.at(i)->GetVertexBufferSRV().GetAddressOf());
		context->CSSetShader(m_csForCollision.Get(), nullptr, 0);
		auto size = blocks.at(i)->GetVertices().size();
		context->Dispatch(blocks.at(i)->GetVertices().size(), 1, 1);
		
		context->CSSetShader(nullptr, nullptr, 0);
		context->CSSetShaderResources(0, 0, nullptr);
		context->CSSetUnorderedAccessViews(0, 0, nullptr, nullptr);
		
		context->CopyResource(debugBuffer, m_sphereResult.Get());
		context->Map(debugBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
		p = reinterpret_cast<Sphere*>(mappedResource.pData);
		context->Unmap(debugBuffer, 0);

		if (m_pos.x != p->center.x || m_pos.z != p->center.z)
		{
			m_pos.x = p->center.x;
			m_pos.z = p->center.z;
		}
	}
}