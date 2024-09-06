#pragma once
#include"GameObject.h"
#include"Animation.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace Microsoft::WRL;
using namespace std;

class GameObject;
class OriginalEffect;

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
		CrouchedWalk	//しゃがみ歩き
	};

	/// <summary>
	/// オブジェクトの初期化
	/// </summary>
	/// <param name="fileName">3Dモデルファイル名</param>
	/// <param name="pos">初期座標</param>
	/// <param name="rotate">オブジェクトのY軸回転量(XM_PI / rotate)</param>
	Player(const wchar_t* fileName, Vector3 pos, float rotate);

	/// <summary>
	/// データ破棄
	/// </summary>
	~Player();

	/// <summary>
	/// オブジェクトの更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// オブジェクトの描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 影用描画
	/// </summary>
	void DrawShadow();

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
	/// <returns>オブジェクトの回転量</returns>
	float GetRotate()const { return m_rotate; }

	/// <summary>
	/// 敵に見つかったかどうか取得
	/// </summary>
	/// <returns></returns>
	bool GetBeFound()const { return m_beFound; }

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	void HitCheckObject();

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
		Vector3 center;
		float radius;
	};
	ComPtr<ID3D11Buffer> m_sphereResult;			//スフィアの入出力バッファ
	ComPtr<ID3D11UnorderedAccessView> m_sphereInfo;	//スフィア用UAV
	ComPtr<ID3D11ComputeShader> m_csForCollision;	//当たり判定用コンピュートシェーダー

	float m_sphereHeight;
	const float m_sphereRadius;
	const float m_sphereDefaultHeight;
	const float m_sphereCrouchHeight;

	bool m_beFound;		//見つかったかどうか

	const float m_scale;
	const float m_speed;		//プレイヤーの移動スピード
	const float m_runSpeed;		//プレイヤーのダッシュスピード
	const float m_crouchSpeed;	//プレイヤーのしゃがみ移動スピード
};

