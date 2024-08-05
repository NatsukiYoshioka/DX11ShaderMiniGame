#include "pch.h"
#include "DeviceAccessor.h"

DeviceAccessor* DeviceAccessor::m_instance = nullptr;

DeviceAccessor::DeviceAccessor(ID3D11Device* device, ID3D11DeviceContext* context, RECT screenSize):
	m_device(device),
	m_context(context),
	m_screenSize(screenSize)
{
	m_states = make_unique<CommonStates>(m_device);
	m_fxFactory = make_unique<EffectFactory>(m_device);
}

DeviceAccessor::~DeviceAccessor()
{
	m_states.reset();
	m_fxFactory.reset();
}

void DeviceAccessor::CreateInstance(ID3D11Device* device, ID3D11DeviceContext* context, RECT screenSize)
{
	if (m_instance)return;
	m_instance = new DeviceAccessor(device, context, screenSize);
}

void DeviceAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}