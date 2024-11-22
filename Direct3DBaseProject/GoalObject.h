#pragma once
#include"GameObject.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace Microsoft::WRL;
using namespace std;

class GameObject;
class OriginalEffect;

class GoalObject:public GameObject
{
public:
	/// <summary>
	/// オブジェクトの初期化
	/// </summary>
	GoalObject(const wchar_t* fileName, Vector3 pos, float rotate);

	/// <summary>
	/// データ破棄
	/// </summary>
	~GoalObject();

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

private:
	shared_ptr<OriginalEffect> m_effect;		//描画用シェーダー
	Matrix m_world;								//ワールド座標行列

	const float m_fixPosZ;		//Z軸調整量
	const Vector3 m_scale;		//オブジェクトのスケール
};

