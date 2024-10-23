#pragma once
using namespace DirectX;
using namespace SimpleMath;
using namespace std;

/// <summary>
/// ゲームオブジェクト基底クラス
/// </summary>
class GameObject abstract
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameObject(){}

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameObject(){}

	/// <summary>
	/// 純粋仮想関数(タイトルシーンオブジェクト初期化関数)
	/// </summary>
	virtual void InitializeTitle() abstract;

	/// <summary>
	/// 純粋仮想関数(タイトルシーンオブジェクト更新関数)
	/// </summary>
	virtual void UpdateTitle() abstract;

	/// <summary>
	/// 純粋仮想関数(タイトルシーンオブジェクト描画関数)
	/// </summary>
	virtual void DrawTitle() abstract;

	/// <summary>
	/// 純粋仮想関数(オブジェクト初期化関数)
	/// </summary>
	virtual void Initialize() abstract;

	/// <summary>
	/// 純粋仮想関数(オブジェクト更新関数)
	/// </summary>
	virtual void Update() abstract;

	/// <summary>
	/// 純粋仮想関数(オブジェクト描画関数)
	/// </summary>
	virtual void Draw() abstract;

	/// <summary>
	/// 純粋仮想関数(リザルトシーンオブジェクト初期化関数)
	/// </summary>
	virtual void InitializeResult() abstract;

	/// <summary>
	/// 純粋仮想関数(リザルトシーンオブジェクト更新関数)
	/// </summary>
	virtual void UpdateResult() abstract;

	/// <summary>
	/// 純粋仮想関数(リザルトシーンオブジェクト描画関数)
	/// </summary>
	virtual void DrawResult() abstract;

	/// <summary>
	/// 純粋仮想関数(影の描画)
	/// </summary>
	virtual void DrawShadow() abstract;

	/// <summary>
	/// 座標の取得
	/// </summary>
	Vector3 GetPos()const { return m_pos; }

	/// <summary>
	/// モデルハンドル取得
	/// </summary>
	Model* GetModelHandle() { return m_modelHandle.get(); }

protected:
	unique_ptr<Model> m_modelHandle;	//3Dモデルハンドル
	Vector3 m_pos;						//オブジェクトの座標
};

