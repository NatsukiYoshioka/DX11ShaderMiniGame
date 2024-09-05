#include "pch.h"
#include "DeviceAccessor.h"

DeviceAccessor* DeviceAccessor::m_instance = nullptr;

//�A�N�Z�T�̏�����
DeviceAccessor::DeviceAccessor(ID3D11Device* device,
	ID3D11DeviceContext* context,
	ID3D11RenderTargetView* rtv,
	ID3D11DepthStencilView* dsv,
	RECT screenSize):
	m_device(device),
	m_context(context),
	m_RTV(rtv),
	m_DSV(dsv),
	m_screenSize(screenSize),
	m_elapsedTime()
{
	m_states = make_unique<CommonStates>(m_device);
	m_fxFactory = make_unique<EffectFactory>(m_device);
	m_gamePad = make_unique<GamePad>();
	m_keyboard = make_unique<Keyboard>();
}

//�f�[�^�j��
DeviceAccessor::~DeviceAccessor()
{
	m_states.reset();
	m_fxFactory.reset();
	m_gamePad.reset();
}

//�C���X�^���X����
void DeviceAccessor::CreateInstance(ID3D11Device* device,
	ID3D11DeviceContext* context,
	ID3D11RenderTargetView* rtv,
	ID3D11DepthStencilView* dsv,
	RECT screenSize)
{
	if (m_instance)return;
	m_instance = new DeviceAccessor(device, context, rtv, dsv, screenSize);
}

//�C���X�^���X�j��
void DeviceAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}