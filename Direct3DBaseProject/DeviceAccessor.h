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
	const D3D11_INPUT_ELEMENT_DESC* GetInputElements() { return m_inputElements; }
	size_t GetInputElementSize() { return size(m_inputElements); }
	void SetElapsedTime(float* elapsedTime) { m_elapsedTime = elapsedTime; }
	float* GetElapsedTime() { return m_elapsedTime; }
private:
	static DeviceAccessor* m_instance;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;
	RECT m_screenSize;
	unique_ptr<CommonStates> m_states;
	unique_ptr<EffectFactory> m_fxFactory;
	float* m_elapsedTime;

	static constexpr const D3D11_INPUT_ELEMENT_DESC m_inputElements[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
};

