#pragma once
using namespace DirectX;
using namespace std;
class DeviceAccessor
{
private:
	DeviceAccessor(ID3D11Device* device, ID3D11DeviceContext* context, RECT screenSize);
	~DeviceAccessor();
public:
	static void CreateInstance(ID3D11Device* device, ID3D11DeviceContext* context, RECT screenSize);
	static void DestroyInstance();
	static DeviceAccessor* GetInstance() { return m_instance; }

	ID3D11Device* GetDevice() { return m_device; }
	ID3D11DeviceContext* GetContext() { return m_context; }
	RECT GetScreenSize() { return m_screenSize; }
	CommonStates* GetStates() { return m_states.get(); }
	EffectFactory* GetEffectFactory() { return m_fxFactory.get(); }
private:
	static DeviceAccessor* m_instance;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;
	RECT m_screenSize;
	unique_ptr<CommonStates> m_states;
	unique_ptr<EffectFactory> m_fxFactory;
};

