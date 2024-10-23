#pragma once
#include"GameObject.h"
#include"Animation.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace Microsoft::WRL;
using namespace std;

class GameObject;
class OriginalEffect;

/// <summary>
/// プレイヤー管理クラス
/// </summary>
class Player :public GameObject
{
public:
	/// <summary>
	/// アニメーションの状態
	/// </summary>
	enum class AnimationState
	{
		Idle,			//待機
		Walk,			//歩き
		Run,			//走り
		Crouch,			//しゃがみ
		CrouchedWalk,	//しゃがみ歩き
		Dance,			//ゲームクリア
		Die				//ゲームオーバー
	};

	/// <summary>
	/// オブジェクトの初期化
	/// </summary>
	Player(const wchar_t* fileName);

	/// <summary>
	/// データ破棄
	/// </summary>
	~Player();

	/// <summary>
	/// タイトルシーンオブジェクトの初期化
	/// </summary>
	void InitializeTitle()override;

	/// <summary>
	/// タイトルシーンオブジェクトの更新
	/// </summary>
	void UpdateTitle()override;

	/// <summary>
	/// タイトルシーンオブジェクトの描画
	/// </summary>
	void DrawTitle()override;

	/// <summary>
	/// オブジェクトの初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// オブジェクトの更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// オブジェクトの描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// リザルトシーンオブジェクトの初期化
	/// </summary>
	void InitializeResult()override;

	/// <summary>
	/// リザルトシーンオブジェクトの更新
	/// </summary>
	void UpdateResult()override;

	/// <summary>
	/// リザルトシーンオブジェクトの描画
	/// </summary>
	void DrawResult()override;

	/// <summary>
	/// 影用描画
	/// </summary>
	void DrawShadow()override;

	/// <summary>
	/// 見つかり判定用描画
	/// </summary>
	void DrawHitCheck();

	/// <summary>
	/// 見つかり判定処理
	/// </summary>
	void HitCheck();

	/// <summary>
	/// オブジェクトの回転量の取得
	/// </summary>
	float GetRotate()const { return m_rotate; }

	/// <summary>
	/// 敵に見つかったかどうか取得
	/// </summary>
	bool GetBeFound()const { return m_beFound; }

	/// <summary>
	/// クリアしたかどうか取得
	/// </summary>
	bool GetIsClear()const { return m_isClear; }

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	void HitCheckObject();

	/// <summary>
	/// アニメーションの状態取得
	/// </summary>
	AnimationState GetNowAnimationState()const { return m_nowAnimationState; }

private:
	AnimationState m_nowAnimationState;			//アニメーションの現在の状態
	vector<DX::AnimationSDKMESH> m_animations;	//スキニングアニメーションクラスコンテナ
	ModelBone::TransformArray m_drawBones;		//ボーン変換行列の配列

	shared_ptr<OriginalEffect> m_effect;		//モデル描画用エフェクトクラス

	float m_rotate;		//モデルのY軸回転量
	Matrix m_world;		//モデルのワールド行列

	/// <summary>
	/// 見つかり判定情報
	/// </summary>
	struct HitInfo
	{
		int playerPixNum;	//見つかった時のプレイヤーのピクセル数
		int visiblePixNum;	//デフォルトピクセル数
	};
	ComPtr<ID3D11Buffer> m_bufferResult;			//見つかり判定入出力用バッファ
	ComPtr<ID3D11UnorderedAccessView> m_hitInfo;	//見つかり判定用UAV
	ComPtr<ID3D11ComputeShader> m_csForEnemyEye;	//見つかり判定用コンピュートシェーダー

	/// <summary>
	/// 当たり判定用スフィア
	/// </summary>
	struct Sphere
	{
		Vector3 center;		//スフィアの中心座標
		float radius;		//スフィアの半径
	};
	ComPtr<ID3D11Buffer> m_sphereResult;			//スフィアの入出力バッファ
	ComPtr<ID3D11UnorderedAccessView> m_sphereInfo;	//スフィア用UAV
	ComPtr<ID3D11ComputeShader> m_csForCollision;	//当たり判定用コンピュートシェーダー

	float m_sphereHeight;				//当たり判定用スフィアの高さ
	const float m_sphereRadius;			//スフィアの半径
	const float m_sphereDefaultHeight;	//スフィアの通常高さ

	bool m_beFound;		//見つかったかどうか
	bool m_isClear;		//クリアしたかどうか

	const Vector3 m_initializeTitlePos;	//タイトルでの初期座標
	const float m_titleRotateX;			//タイトルでの初期X軸回転量
	const float m_titleRotateY;			//タイトルでの初期Y軸回転量
	const float m_titleScale;			//タイトルでのスケール

	const Vector3 m_initializePos;		//ゲームシーンのオブジェクト初期座標
	const float m_initializeRotate;		//ゲームシーンのオブジェクト初期回転量
	const float m_scale;				//プレイヤーのスケール
	const float m_speed;				//プレイヤーの移動スピード
	const float m_runSpeed;				//プレイヤーのダッシュスピード
	const float m_crouchSpeed;			//プレイヤーのしゃがみ移動スピード

	const Vector3 m_clearPos;			//クリア時の初期座標
	const Vector3 m_clearFinalPos;		//クリア時の最終座標
};

