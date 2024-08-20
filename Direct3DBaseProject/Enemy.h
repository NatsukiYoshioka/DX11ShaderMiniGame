#pragma once
#include"GameObject.h"
#include"Animation.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace Microsoft::WRL;
using namespace std;

class GameObject;
class OriginalEffect;

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
	Enemy(const wchar_t* fileName, Vector3 pos, float rotate);
	~Enemy();

	void Update()override;
	void Draw()override;

	Vector3 GetEyePosition()const { return m_eyePos; }
	Vector3 GetEyeDirection()const { return m_eyeDirection; }

private:
	AnimationState m_nowAnimationState;			//アニメーションの現在の状態
	vector<DX::AnimationSDKMESH> m_animations;	//スキニングアニメーションクラスコンテナ
	ModelBone::TransformArray m_drawBones;		//ボーン変換行列の配列

	shared_ptr<OriginalEffect> m_effect;		//モデル描画用エフェクトクラス

	Vector3 m_eyePos;		//敵の目の位置
	Vector3 m_eyeDirection;	//目の向き

	float m_posAngle;	//モデルの座標を決める角度
	float m_rotate;		//モデルのY軸回転量
	Matrix m_world;		//モデルのワールド行列

	const float m_scale;	//モデルのスケール
	const float m_distance;	//モデルの距離
	const float m_speed;	//モデルの移動スピード
};

