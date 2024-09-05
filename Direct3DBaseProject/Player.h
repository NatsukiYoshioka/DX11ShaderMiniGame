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

	void DrawShadow();

	void DrawHitCheck();

	void HitCheck();

	/// <summary>
	/// オブジェクトの回転量の取得
	/// </summary>
	/// <returns>オブジェクトの回転量</returns>
	float GetRotate()const { return m_rotate; }

	bool GetBeFound()const { return m_beFound; }

	void HitCheckObject();

private:
	AnimationState m_nowAnimationState;			//アニメーションの現在の状態
	vector<DX::AnimationSDKMESH> m_animations;	//スキニングアニメーションクラスコンテナ
	ModelBone::TransformArray m_drawBones;		//ボーン変換行列の配列

	shared_ptr<OriginalEffect> m_effect;		//モデル描画用エフェクトクラス

	float m_rotate;		//モデルのY軸回転量
	Matrix m_world;		//モデルのワールド行列

	struct HitInfo
	{
		int playerPixNum;
		int visiblePixNum;
	};
	ComPtr<ID3D11Buffer> m_bufferResult;
	ComPtr<ID3D11UnorderedAccessView> m_hitInfo;
	ComPtr<ID3D11ComputeShader> m_csForEnemyEye;

	struct Sphere
	{
		Vector3 center;
		float radius;
	};
	ComPtr<ID3D11Buffer> m_sphereResult;
	ComPtr<ID3D11UnorderedAccessView> m_sphereInfo;
	ComPtr<ID3D11ComputeShader> m_csForCollision;

	bool m_beFound;

	const float m_scale;
	const float m_speed;		//プレイヤーの移動スピード
	const float m_runSpeed;		//プレイヤーのダッシュスピード
	const float m_crouchSpeed;	//プレイヤーのしゃがみ移動スピード

	unique_ptr<SpriteBatch> batch;
	unique_ptr<SpriteFont> font;
};

