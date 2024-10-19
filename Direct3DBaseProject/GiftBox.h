#pragma once
#include"GameObject.h"

class GameObject;
class OriginalEffect;

class GiftBox:public GameObject
{
public:
	/// <summary>
	/// オブジェクトの初期化
	/// </summary>
	/// <param name="fileName"></param>
	/// <param name="pos"></param>
	/// <param name="rotate"></param>
	GiftBox(const wchar_t* fileName, Vector3 pos, float rotate);

	/// <summary>
	/// データ破棄
	/// </summary>
	~GiftBox();

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
	/// 影の描画
	/// </summary>
	void DrawShadow()override;
private:
	shared_ptr<OriginalEffect> m_effect;		//モデル描画用エフェクトクラス

	float m_rotate;		//オブジェクトの回転量
	Matrix m_world;		//モデルのワールド行列
};

