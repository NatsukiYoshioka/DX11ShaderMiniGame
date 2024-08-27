#pragma once
using namespace DirectX;
using namespace std;
class DeviceAccessor
{
private:
	/// <summary>
	/// アクセサの初期化
	/// </summary>
	/// <param name="device">描画デバイス</param>
	/// <param name="context">描画コンテキスト</param>
	/// <param name="screenSize">スクリーンサイズ</param>
	DeviceAccessor(ID3D11Device* device,
		ID3D11DeviceContext* context,
		ID3D11RenderTargetView* rtv,
		ID3D11DepthStencilView* dsv,
		RECT screenSize);

	/// <summary>
	/// データ破棄
	/// </summary>
	~DeviceAccessor();
public:
	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <param name="device">描画デバイス</param>
	/// <param name="context">描画コンテキスト</param>
	/// <param name="screenSize">スクリーンサイズ</param>
	static void CreateInstance(ID3D11Device* device,
		ID3D11DeviceContext* context,
		ID3D11RenderTargetView* rtv,
		ID3D11DepthStencilView* dsv,
		RECT screenSize);

	/// <summary>
	/// インスタンス破棄
	/// </summary>
	static void DestroyInstance();

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>クラスのインスタンス</returns>
	static DeviceAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// 描画デバイスの取得
	/// </summary>
	/// <returns>描画デバイス</returns>
	ID3D11Device* GetDevice() { return m_device; }

	/// <summary>
	/// 描画コンテキストの取得
	/// </summary>
	/// <returns>描画コンテキスト</returns>
	ID3D11DeviceContext* GetContext() { return m_context; }

	ID3D11RenderTargetView* GetRenderTargetView() { return m_RTV; }

	ID3D11DepthStencilView* GetDepthStencilView() { return m_DSV; }

	/// <summary>
	/// スクリーンサイズの取得
	/// </summary>
	/// <returns>スクリーンサイズ</returns>
	RECT GetScreenSize()const { return m_screenSize; }

	/// <summary>
	/// D3Dステートオブジェクトの取得
	/// </summary>
	/// <returns>CommonStates*</returns>
	CommonStates* GetStates() { return m_states.get(); }

	/// <summary>
	/// エフェクトヘルパーオブジェクトの取得
	/// </summary>
	/// <returns>EffectFactory*</returns>
	EffectFactory* GetEffectFactory() { return m_fxFactory.get(); }

	/// <summary>
	/// 経過時間の設定
	/// </summary>
	/// <param name="elapsedTime">経過時間 float*</param>
	void SetElapsedTime(float* elapsedTime) { m_elapsedTime = elapsedTime; }

	/// <summary>
	/// 経過時間の取得
	/// </summary>
	/// <returns>float*</returns>
	float* GetElapsedTime() { return m_elapsedTime; }

	/// <summary>
	/// ゲームパッド状態の取得
	/// </summary>
	/// <returns>GamePad*</returns>
	GamePad* GetGamePad() { return m_gamePad.get(); }
private:
	static DeviceAccessor* m_instance;		//クラスのインスタンス

	ID3D11Device* m_device;					//描画デバイス
	ID3D11DeviceContext* m_context;			//描画コンテキスト
	ID3D11RenderTargetView* m_RTV;
	ID3D11DepthStencilView* m_DSV;
	RECT m_screenSize;						//スクリーンサイズ
	unique_ptr<CommonStates> m_states;		//D3Dステートオブジェクト
	unique_ptr<EffectFactory> m_fxFactory;	//エフェクトヘルパーオブジェクト
	float* m_elapsedTime;					//経過時間
	unique_ptr<GamePad> m_gamePad;			//ゲームパッド
};

