#pragma once
#include"GameObject.h"

class GameObject;
class OriginalEffect;

/// <summary>
/// 人形の頭管理クラス
/// </summary>
class DollHead:public GameObject
{
public:
	/// <summary>
	/// オブジェクトの初期化
	/// </summary>
	DollHead(const wchar_t* fileName, Vector3 pos);

	/// <summary>
	/// オブジェクトの破棄
	/// </summary>
	~DollHead();

	/// <summary>
	/// タイトルでの初期化
	/// </summary>
	void InitializeTitle()override;

	/// <summary>
	/// タイトルでの更新
	/// </summary>
	void UpdateTitle()override;

	/// <summary>
	/// タイトルでの描画
	/// </summary>
	void DrawTitle()override;

	/// <summary>
	/// 初期化
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
	/// リザルトでの初期化
	/// </summary>
	void InitializeResult()override;

	/// <summary>
	/// リザルトでの更新
	/// </summary>
	void UpdateResult()override;

	/// <summary>
	/// リザルトでの描画
	/// </summary>
	void DrawResult()override;

	/// <summary>
	/// 影の描画
	/// </summary>
	void DrawShadow()override;

	/// <summary>
	/// リザルトでの座標移動割合取得
	/// </summary>
	float GetPosRatio()const { return m_posRatio; }

private:
	shared_ptr<OriginalEffect> m_effect;	//シェーダー描画用クラス
	Matrix m_world;							//ワールド行列
	float m_speed;							//移動スピード
	float m_rotate;							//回転量
	float m_rotationSpeed;					//回転スピード

	float m_posRatio;						//座標移動割合

	const float m_scale;					//モデルのスケール
	const Vector3 m_firstPos;				//初期座標
	const Vector3 m_finalPos;				//最終座標
	const float m_firstSpeed;				//初期スピード
	const float m_subSpeed;					//スピード減少量
	const float m_firstRotationSpeed;		//初期回転スピード
	const float m_subRotationSpeed;			//回転スピード減少量
};