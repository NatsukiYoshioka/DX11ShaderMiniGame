#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include"Enemy.h"
#include"EnemyAccessor.h"
#include"Block.h"
#include"BlockAccessor.h"
#include"UIBase.h"
#include"FoundUI.h"
#include"UIAccessor.h"
#include"OriginalEffect.h"
#include"GameObject.h"
#include"ReadData.h"
#include"GameObjectManager.h"
#include "Player.h"

//オブジェクトの初期化
Player::Player(const wchar_t* fileName):
	m_rotate(),
	m_nowAnimationState(AnimationState::Idle),
	m_beFound(false),
	m_isClear(false),
	m_initializeTitlePos(Vector3(Json::GetInstance()->GetData()["PlayerTitlePosition"].at(0),
		Json::GetInstance()->GetData()["PlayerTitlePosition"].at(1),
		Json::GetInstance()->GetData()["PlayerTitlePosition"].at(2))),
	m_titleRotateX(float(Json::GetInstance()->GetData()["PlayerTitleRotateX"])),
	m_titleRotateY(float()),
	m_titleScale(float(Json::GetInstance()->GetData()["PlayerTitleScale"])),
	m_initializePos(Vector3(Json::GetInstance()->GetData()["PlayerPosition"].at(0),
		Json::GetInstance()->GetData()["PlayerPosition"].at(1),
		Json::GetInstance()->GetData()["PlayerPosition"].at(2))),
	m_initializeRotate(float(Json::GetInstance()->GetData()["PlayerPosition"].at(3))* XM_PI / 180.f),
	m_sphereRadius(float(Json::GetInstance()->GetData()["SphereRadius"])),
	m_sphereDefaultHeight(float(Json::GetInstance()->GetData()["SphereDefaultHeight"])),
	m_scale(float(Json::GetInstance()->GetData()["PlayerScale"])),
	m_speed(float(Json::GetInstance()->GetData()["PlayerSpeed"])),
	m_runSpeed(float(Json::GetInstance()->GetData()["PlayerRunSpeed"])),
	m_crouchSpeed(float(Json::GetInstance()->GetData()["PlayerCrouchSpeed"])),
	m_clearPos(Vector3(Json::GetInstance()->GetData()["PlayerClearPosition"].at(0),
		Json::GetInstance()->GetData()["PlayerClearPosition"].at(1),
		Json::GetInstance()->GetData()["PlayerClearPosition"].at(2))),
	m_clearFinalPos(Vector3(Json::GetInstance()->GetData()["PlayerClearFinalPosition"].at(0),
		Json::GetInstance()->GetData()["PlayerClearFinalPosition"].at(1),
		Json::GetInstance()->GetData()["PlayerClearFinalPosition"].at(2)))
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
	m_sphereHeight = m_sphereDefaultHeight;

	//入出力UAV設定
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

//データ破棄
Player::~Player()
{
	m_modelHandle.reset();
}

//タイトルシーンオブジェクトの初期化
void Player::InitializeTitle()
{
	m_pos = m_initializeTitlePos;
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(m_titleScale));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationX(m_titleRotateX));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));
	m_nowAnimationState = AnimationState::Idle;
	m_animations.at(static_cast<int>(AnimationState::Die)).ResetAnimTime();
}

//タイトルシーンオブジェクトの更新
void Player::UpdateTitle()
{
	//ライト用情報設定
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
}

//タイトルシーンオブジェクトの描画
void Player::DrawTitle()
{
	Draw();
}

//オブジェクト初期化
void Player::Initialize()
{
	m_pos = m_initializePos;
	m_rotate = m_initializeRotate;
	m_beFound = false;
	m_isClear = false;
}

//オブジェクト更新
void Player::Update()
{
	auto pad = DeviceAccessor::GetInstance()->GetGamePad()->GetState(0);
	auto key = DeviceAccessor::GetInstance()->GetKeyboard()->GetState();
	auto mouse = DeviceAccessor::GetInstance()->GetMouse()->GetState();

	bool isMove = false;
	bool isCrouch = false;
	float nowSpeed = 0.f;
	m_nowAnimationState = AnimationState::Idle;

	//移動処理
	FoundUI* foundUI = nullptr;
	for (int i = 0; i < UIAccessor::GetInstance()->GetUIs().size(); i++)
	{
		foundUI = dynamic_cast<FoundUI*>(UIAccessor::GetInstance()->GetUIs().at(i));
		if (foundUI)break;
	}
	//完全に見つかっていなければ移動処理を行う
	if (foundUI && !foundUI->GetIsFound() && !m_isClear)
	{
		if (pad.IsLeftShoulderPressed() || key.LeftControl || mouse.rightButton)
		{
			m_nowAnimationState = AnimationState::Crouch;
			isCrouch = true;
		}
		if (pad.thumbSticks.leftX != 0 || pad.thumbSticks.leftY != 0 || key.W || key.A || key.S || key.D)
		{
			float x = -pad.thumbSticks.leftX;
			float y = pad.thumbSticks.leftY;
			if (key.W)
			{
				y = 1;
			}
			if (key.S)
			{
				y = -1;
			}
			if (key.A)
			{
				x = 1;
			}
			if (key.D)
			{
				x = -1;
			}
			m_rotate = atan2f(x, y);


			isMove = true;
			nowSpeed = m_speed;
			m_nowAnimationState = AnimationState::Walk;

			if (isCrouch)
			{
				nowSpeed = m_crouchSpeed;
				m_nowAnimationState = AnimationState::CrouchedWalk;
			}
			else if (pad.IsRightShoulderPressed() || key.LeftShift || mouse.leftButton)
			{
				nowSpeed = m_runSpeed;
				m_nowAnimationState = AnimationState::Run;
			}
		}
	}
	else if(foundUI && foundUI->GetIsFound())
	{
		m_nowAnimationState = AnimationState::Die;
	}

	if (isMove)
	{
		//座標計算処理
		auto cameraPitch = CameraAccessor::GetInstance()->GetCamera()->GetPitch();
		auto radian = cameraPitch * XM_PI / 180;
		m_rotate += radian;

		m_pos.z += -cos(m_rotate) * nowSpeed;
		m_pos.x += -sin(m_rotate) * nowSpeed;
	}

	if (m_pos.x > m_clearPos.x && m_pos.z > m_clearPos.z)
	{
		m_isClear = true;
	}

	//ライト用情報設定
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

	//当たり判定処理
	HitCheckObject();

	//ワールド座標行列の更新
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(m_scale));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationY(m_rotate));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));

	if (m_nowAnimationState == AnimationState::Die)
	{
		m_animations.at(static_cast<int>(m_nowAnimationState)).Update(*DeviceAccessor::GetInstance()->GetElapsedTime()/2.5);
	}
	else
	{
		m_animations.at(static_cast<int>(m_nowAnimationState)).Update(*DeviceAccessor::GetInstance()->GetElapsedTime());
	}
}

//オブジェクト描画
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

//リザルトシーンオブジェクトの初期化
void Player::InitializeResult()
{
	m_nowAnimationState = AnimationState::Run;
	m_pos = Vector3(
		Json::GetInstance()->GetData()["PlayerResultPosition"].at(0),
		Json::GetInstance()->GetData()["PlayerResultPosition"].at(1),
		Json::GetInstance()->GetData()["PlayerResultPosition"].at(2)
	);
	m_rotate = float(Json::GetInstance()->GetData()["PlayerResultRotationY"]) * XM_PI / 180;
}

//リザルトシーンオブジェクトの更新
void Player::UpdateResult()
{
	if (!m_isClear)return;
	if (m_pos.z <= m_clearFinalPos.z)
	{
		m_pos.z += -cos(m_rotate) * m_runSpeed;
		m_pos.x += -sin(m_rotate) * m_runSpeed;
	}
	else
	{
		m_nowAnimationState = AnimationState::Dance;
	}

	//ライト用情報設定
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

	//ワールド座標行列の更新
	m_world = Matrix::Identity;
	m_world = XMMatrixMultiply(m_world, Matrix::CreateScale(m_scale));
	m_world = XMMatrixMultiply(m_world, Matrix::CreateRotationY(m_rotate));
	m_world = XMMatrixMultiply(m_world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));

	m_animations.at(static_cast<int>(m_nowAnimationState)).Update(*DeviceAccessor::GetInstance()->GetElapsedTime());
}

//リザルトシーンオブジェクトの描画
void Player::DrawResult()
{
	if (!m_isClear)return;
	Draw();
}

//オブジェクトの影用描画
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

//見つかり判定用描画
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

//見つかり判定処理
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
	assert(debugBuffer);
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

//当たり判定処理
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
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;  //CPUから読み込みできるように設定する
		BufferDesc.Usage = D3D11_USAGE_STAGING;             //GPUからCPUへのデータ転送(コピー)をサポートするリソース
		BufferDesc.BindFlags = 0;
		BufferDesc.MiscFlags = 0;
		device->CreateBuffer(&BufferDesc, NULL, &debugBuffer);
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		assert(debugBuffer);
		context->Map(debugBuffer, 0, D3D11_MAP_WRITE, 0, &mappedResource);
		Sphere* p = reinterpret_cast<Sphere*>(mappedResource.pData);
		p->center = m_pos;
		p->center.y = m_sphereHeight;
		p->radius = m_sphereRadius;
		context->Unmap(debugBuffer, 0);
		context->CopyResource(m_sphereResult.Get(), debugBuffer);
		
		context->CSSetUnorderedAccessViews(0, 1, m_sphereInfo.GetAddressOf(), 0);
		context->CSSetShaderResources(0, 1, blocks.at(i)->GetVertexBufferSRV().GetAddressOf());
		context->CSSetShader(m_csForCollision.Get(), nullptr, 0);
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