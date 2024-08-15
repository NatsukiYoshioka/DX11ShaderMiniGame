#pragma once
using namespace DirectX;
using namespace std;
class DeviceAccessor
{
private:
	/// <summary>
	/// �A�N�Z�T�̏�����
	/// </summary>
	/// <param name="device">�`��f�o�C�X</param>
	/// <param name="context">�`��R���e�L�X�g</param>
	/// <param name="screenSize">�X�N���[���T�C�Y</param>
	DeviceAccessor(ID3D11Device* device, ID3D11DeviceContext* context, RECT screenSize);

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~DeviceAccessor();
public:
	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	/// <param name="device">�`��f�o�C�X</param>
	/// <param name="context">�`��R���e�L�X�g</param>
	/// <param name="screenSize">�X�N���[���T�C�Y</param>
	static void CreateInstance(ID3D11Device* device, ID3D11DeviceContext* context, RECT screenSize);

	/// <summary>
	/// �C���X�^���X�j��
	/// </summary>
	static void DestroyInstance();

	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns>�N���X�̃C���X�^���X</returns>
	static DeviceAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// �`��f�o�C�X�̎擾
	/// </summary>
	/// <returns>�`��f�o�C�X</returns>
	ID3D11Device* GetDevice() { return m_device; }

	/// <summary>
	/// �`��R���e�L�X�g�̎擾
	/// </summary>
	/// <returns>�`��R���e�L�X�g</returns>
	ID3D11DeviceContext* GetContext() { return m_context; }

	/// <summary>
	/// �X�N���[���T�C�Y�̎擾
	/// </summary>
	/// <returns>�X�N���[���T�C�Y</returns>
	RECT GetScreenSize()const { return m_screenSize; }

	/// <summary>
	/// D3D�X�e�[�g�I�u�W�F�N�g�̎擾
	/// </summary>
	/// <returns>CommonStates*</returns>
	CommonStates* GetStates() { return m_states.get(); }

	/// <summary>
	/// �G�t�F�N�g�w���p�[�I�u�W�F�N�g�̎擾
	/// </summary>
	/// <returns>EffectFactory*</returns>
	EffectFactory* GetEffectFactory() { return m_fxFactory.get(); }

	/// <summary>
	/// ���̓Z�}���e�B�N�X�̎擾
	/// </summary>
	/// <returns>D3D11_ELEMENT_DESC*</returns>
	const D3D11_INPUT_ELEMENT_DESC* GetInputElements() { return m_inputElements; }

	/// <summary>
	/// ���̓Z�}���e�B�N�X�̃T�C�Y�擾
	/// </summary>
	/// <returns>size_t</returns>
	size_t GetInputElementSize() { return size(m_inputElements); }

	/// <summary>
	/// �o�ߎ��Ԃ̐ݒ�
	/// </summary>
	/// <param name="elapsedTime">�o�ߎ��� float*</param>
	void SetElapsedTime(float* elapsedTime) { m_elapsedTime = elapsedTime; }

	/// <summary>
	/// �o�ߎ��Ԃ̎擾
	/// </summary>
	/// <returns>float*</returns>
	float* GetElapsedTime() { return m_elapsedTime; }

	/// <summary>
	/// �Q�[���p�b�h��Ԃ̎擾
	/// </summary>
	/// <returns>GamePad*</returns>
	GamePad* GetGamePad() { return m_gamePad.get(); }
private:
	static DeviceAccessor* m_instance;		//�N���X�̃C���X�^���X

	ID3D11Device* m_device;					//�`��f�o�C�X
	ID3D11DeviceContext* m_context;			//�`��R���e�L�X�g
	RECT m_screenSize;						//�X�N���[���T�C�Y
	unique_ptr<CommonStates> m_states;		//D3D�X�e�[�g�I�u�W�F�N�g
	unique_ptr<EffectFactory> m_fxFactory;	//�G�t�F�N�g�w���p�[�I�u�W�F�N�g
	float* m_elapsedTime;					//�o�ߎ���
	unique_ptr<GamePad> m_gamePad;			//�Q�[���p�b�h

	/// <summary>
	/// ���̓Z�}���e�B�N�X��`
	/// </summary>
	static constexpr const D3D11_INPUT_ELEMENT_DESC m_inputElements[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
};

