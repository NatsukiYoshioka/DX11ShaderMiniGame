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
/// エネミーオブジェクト
/// </summary>
class Enemy :public GameObject
{
public:
	/// <summary>
	/// アニメーションの状態
	/// </summary>
	enum class AnimationState
	{
		Idle,			//待機
		RightWalk,		//右歩き
		LeftWalk		//左歩き
	};

	/// <summary>
	/// オブジェクトの初期化
	/// </summary>
	/// <param name="fileName">3Dモデルファイル名</param>
	/// <param name="pos">初期座標</param>
	/// <param name="rotate">初期回転量</param>
	Enemy(const wchar_t* fileName);

	/// <summary>
	/// データ破棄
	/// </summary>
	~Enemy();

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
	/// 敵の目の位置を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetEyePosition()const { return m_eyePos; }

	/// <summary>
	/// 敵の目の向きを取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetEyeDirection()const { return m_eyeDirection; }

	/// <summary>
	/// 敵の目のビュー空間行列
	/// </summary>
	/// <returns></returns>
	Matrix GetEyeView()const { return m_eyeView; }

private:
	AnimationState m_nowAnimationState;			//アニメーションの現在の状態
	vector<DX::AnimationSDKMESH> m_animations;	//スキニングアニメーションクラスコンテナ
	ModelBone::TransformArray m_drawBones;		//ボーン変換行列の配列

	shared_ptr<OriginalEffect> m_effect;		//モデル描画用エフェクトクラス

	Vector3 m_eyePos;		//敵の目の位置
	Vector3 m_eyeDirection;	//目の向き
	Matrix m_eyeView;		//敵の目のビュー行列

	float m_posAngle;	//モデルの座標を決める角度
	float m_rotate;		//モデルのY軸回転量
	Matrix m_world;		//モデルのワールド行列

	bool m_isStartMoving;

	const Vector3 m_initializeTitlePos;	//モデルのタイトルシーン初期座標
	const Vector3 m_initializePos;		//モデルのゲームシーン初期座標
	const float m_initializeTitleRotate;//モデルのタイトルシーン初期回転量
	const float m_initializeRotate;		//モデルのゲームシーン初期回転量
	const float m_scale;				//モデルのスケール
	const float m_distance;				//モデルの距離
	const float m_speed;				//モデルの移動スピード
};

