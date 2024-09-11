#pragma once
using namespace DirectX;
using namespace SimpleMath;
using namespace std;

/// <summary>
/// ゲームオブジェクト基底クラス
/// </summary>
class GameObject
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
	virtual void InitializeTitle() = 0;

	/// <summary>
	/// 純粋仮想関数(タイトルシーンオブジェクト更新関数)
	/// </summary>
	virtual void UpdateTitle() = 0;

	/// <summary>
	/// 純粋仮想関数(タイトルシーンオブジェクト描画関数)
	/// </summary>
	virtual void DrawTitle() = 0;

	/// <summary>
	/// 純粋仮想関数(オブジェクト初期化関数)
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 純粋仮想関数(オブジェクト更新関数)
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 純粋仮想関数(オブジェクト描画関数)
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 純粋仮想関数(リザルトシーンオブジェクト初期化関数)
	/// </summary>
	virtual void InitializeResult() = 0;

	/// <summary>
	/// 純粋仮想関数(リザルトシーンオブジェクト更新関数)
	/// </summary>
	virtual void UpdateResult() = 0;

	/// <summary>
	/// 純粋仮想関数(リザルトシーンオブジェクト描画関数)
	/// </summary>
	virtual void DrawResult() = 0;

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標(Vector3)</returns>
	Vector3 GetPos()const { return m_pos; }

	/// <summary>
	/// モデルハンドル取得
	/// </summary>
	/// <returns>モデルのポインタ</returns>
	Model* GetModelHandle() { return m_modelHandle.get(); }

protected:
	unique_ptr<Model> m_modelHandle;	//3Dモデルハンドル
	Vector3 m_pos;						//オブジェクトの座標
};

