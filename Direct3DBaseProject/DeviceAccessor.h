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
	DeviceAccessor(ID3D11Device* device,
		ID3D11DeviceContext* context,
		ID3D11RenderTargetView* rtv,
		ID3D11DepthStencilView* dsv,
		RECT screenSize,
		AudioEngine* audioEngine);

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
	static void CreateInstance(ID3D11Device* device,
		ID3D11DeviceContext* context,
		ID3D11RenderTargetView* rtv,
		ID3D11DepthStencilView* dsv,
		RECT screenSize,
		AudioEngine* audioEngine);

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
	/// �f�t�H���g�̃����_�[�^�[�Q�b�g���擾
	/// </summary>
	/// <returns></returns>
	ID3D11RenderTargetView* GetRenderTargetView() { return m_RTV; }

	/// <summary>
	/// �f�t�H���g�̐[�x�e�X�g�f�o�C�X���擾
	/// </summary>
	/// <returns></returns>
	ID3D11DepthStencilView* GetDepthStencilView() { return m_DSV; }

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

	/// <summary>
	/// �L�[�{�[�h����N���X�擾
	/// </summary>
	/// <returns></returns>
	Keyboard* GetKeyboard() { return m_keyboard.get(); }

	/// <summary>
	/// �}�E�X����N���X�̐ݒ�
	/// </summary>
	/// <param name="mouse"></param>
	void SetMouse(Mouse* mouse) { m_mouse = mouse; }

	/// <summary>
	/// �܂�������N���X�̎擾
	/// </summary>
	/// <returns></returns>
	Mouse* GetMouse() { return m_mouse; }

	/// <summary>
	/// �I�[�f�B�I����G���W���̎擾
	/// </summary>
	/// <returns></returns>
	AudioEngine* GetAudioEngine() { return m_audioEngine; }
private:
	static DeviceAccessor* m_instance;				//�N���X�̃C���X�^���X

	ID3D11Device* m_device;							//�`��f�o�C�X
	ID3D11DeviceContext* m_context;					//�`��R���e�L�X�g
	ID3D11RenderTargetView* m_RTV;
	ID3D11DepthStencilView* m_DSV;
	RECT m_screenSize;								//�X�N���[���T�C�Y
	unique_ptr<CommonStates> m_states;				//D3D�X�e�[�g�I�u�W�F�N�g
	unique_ptr<EffectFactory> m_fxFactory;			//�G�t�F�N�g�w���p�[�I�u�W�F�N�g
	float* m_elapsedTime;							//�o�ߎ���
	unique_ptr<GamePad> m_gamePad;					//�Q�[���p�b�h
	unique_ptr<Keyboard> m_keyboard;				//�L�[�{�[�h
	Mouse* m_mouse;									//�}�E�X
	DirectX::AudioEngine* m_audioEngine;			//�I�[�f�B�I�Đ��p�G���W��
};

