#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"OriginalEffect.h"
#include"Camera.h"
#include"CameraAccessor.h"
#include "GoalObject.h"

GoalObject::GoalObject():
	m_size(XMFLOAT3(
		Json::GetInstance()->GetData()["GoalBoxScale"].at(0),
		Json::GetInstance()->GetData()["GoalBoxScale"].at(1),
		Json::GetInstance()->GetData()["GoalBoxScale"].at(2)
	)),
	m_colorBuffer(DeviceAccessor::GetInstance()->GetDevice())
{
	auto deviceAccessor = DeviceAccessor::GetInstance();
	SetCurrentDirectory(L"Assets/Shader");
	m_effect = make_unique<OriginalEffect>(deviceAccessor->GetDevice(), OriginalEffect::PixelType::Light);
	SetCurrentDirectory(L"../../");
	m_box = GeometricPrimitive::CreateBox(deviceAccessor->GetContext(), m_size);
	m_box->CreateInputLayout(m_effect.get(), m_inputLayout.ReleaseAndGetAddressOf());

	m_pos = Vector3(
		Json::GetInstance()->GetData()["PlayerClearPosition"].at(0),
		Json::GetInstance()->GetData()["PlayerClearPosition"].at(1),
		Json::GetInstance()->GetData()["PlayerClearPosition"].at(2)
	);
}

GoalObject::~GoalObject()
{

}

//タイトルでの初期化(処理なし)
void GoalObject::InitializeTitle()
{

}

//タイトルでの更新(処理なし)
void GoalObject::UpdateTitle()
{

}

//タイトルでの描画(処理なし)
void GoalObject::DrawTitle()
{
	
}

//ゲームシーンの初期化(処理なし)
void GoalObject::Initialize()
{

}

//ゲームシーンの更新(処理なし)
void GoalObject::Update()
{

}

//ゲームシーンの描画
void GoalObject::Draw()
{
	auto world = Matrix::Identity;
	world = XMMatrixMultiply(world, XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));
	m_effect->Apply(DeviceAccessor::GetInstance()->GetContext());
	Color color;
	color.color = Colors::White;
	m_colorBuffer.SetData(DeviceAccessor::GetInstance()->GetContext(), color);
	auto cb = m_colorBuffer.GetBuffer();
	DeviceAccessor::GetInstance()->GetContext()->PSSetConstantBuffers(4, 1, &cb);
	m_box->Draw(
		world,
		CameraAccessor::GetInstance()->GetCamera()->GetView(),
		CameraAccessor::GetInstance()->GetCamera()->GetProjection()
		);
}

//リザルトシーンでの初期化
void GoalObject::InitializeResult()
{
	
}

//リザルトシーンでの更新
void GoalObject::UpdateResult()
{
	
}

//リザルトでの描画
void GoalObject::DrawResult()
{
	
}

//影の描画(処理なし)
void GoalObject::DrawShadow()
{
	
}