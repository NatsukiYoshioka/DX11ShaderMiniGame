#pragma once
using namespace DirectX;
using namespace std;
class DeviceAccessor
{
private:
	/// <summary>
	/// アクセサの初期化
	/// </summary>
	DeviceAccessor(ID3D11Device* device,
		ID3D11DeviceContext* context,
		ID3D11RenderTargetView* rtv,
		ID3D11DepthStencilView* dsv,
		RECT screenSize,
		AudioEngine* audioEngine);

	/// <summary>
	/// データ破棄
	/// </summary>
	~DeviceAccessor();
public:
	/// <summary>
	/// インスタンス生成
	/// </summary>
	static void CreateInstance(ID3D11Device* device,
		ID3D11DeviceContext* context,
		ID3D11RenderTargetView* rtv,
		ID3D11DepthStencilView* dsv,
		RECT screenSize,
		AudioEngine* audioEngine);

	/// <summary>
	/// インスタンス破棄
	/// </summary>
	static void DestroyInstance();

	/// <summary>
	/// インスタンス取得
	/// </summary>
	static DeviceAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// 描画デバイスの取得
	/// </summary>
	ID3D11Device* GetDevice() { return m_device; }

	/// <summary>
	/// 描画コンテキストの取得
	/// </summary>
	ID3D11DeviceContext* GetContext() { return m_context; }

	/// <summary>
	/// デフォルトのレンダーターゲットを取得
	/// </summary>
	ID3D11RenderTargetView* GetRenderTargetView() { return m_RTV; }

	/// <summary>
	/// デフォルトの深度テストデバイスを取得
	/// </summary>
	ID3D11DepthStencilView* GetDepthStencilView() { return m_DSV; }

	/// <summary>
	/// スクリーンサイズの取得
	/// </summary>
	RECT GetScreenSize()const { return m_screenSize; }

	/// <summary>
	/// D3Dステートオブジェクトの取得
	/// </summary>
	CommonStates* GetStates() { return m_states.get(); }

	/// <summary>
	/// エフェクトヘルパーオブジェクトの取得
	/// </summary>
	EffectFactory* GetEffectFactory() { return m_fxFactory.get(); }

	/// <summary>
	/// 経過時間の設定
	/// </summary>
	void SetElapsedTime(float* elapsedTime) { m_elapsedTime = elapsedTime; }

	/// <summary>
	/// 経過時間の取得
	/// </summary>
	float* GetElapsedTime() { return m_elapsedTime; }

	/// <summary>
	/// ゲームパッド状態の取得
	/// </summary>
	GamePad* GetGamePad() { return m_gamePad.get(); }

	/// <summary>
	/// キーボード制御クラス取得
	/// </summary>
	Keyboard* GetKeyboard() { return m_keyboard.get(); }

	/// <summary>
	/// マウス制御クラスの設定
	/// </summary>
	void SetMouse(Mouse* mouse) { m_mouse = mouse; }

	/// <summary>
	/// マウス制御クラスの取得
	/// </summary>
	Mouse* GetMouse() { return m_mouse; }

	/// <summary>
	/// オーディオ制御エンジンの取得
	/// </summary>
	AudioEngine* GetAudioEngine() { return m_audioEngine; }
private:
	static DeviceAccessor* m_instance;				//クラスのインスタンス

	ID3D11Device* m_device;							//描画デバイス
	ID3D11DeviceContext* m_context;					//描画コンテキスト
	ID3D11RenderTargetView* m_RTV;					//レンダーターゲットビュー
	ID3D11DepthStencilView* m_DSV;					//深度ステンシルビュー
	RECT m_screenSize;								//スクリーンサイズ
	unique_ptr<CommonStates> m_states;				//D3Dステートオブジェクト
	unique_ptr<EffectFactory> m_fxFactory;			//エフェクトヘルパーオブジェクト
	float* m_elapsedTime;							//経過時間
	unique_ptr<GamePad> m_gamePad;					//ゲームパッド
	unique_ptr<Keyboard> m_keyboard;				//キーボード
	Mouse* m_mouse;									//マウス
	DirectX::AudioEngine* m_audioEngine;			//オーディオ再生用エンジン
};

