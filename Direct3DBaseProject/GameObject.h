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
	/// 純粋仮想関数(オブジェクト更新関数)
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 純粋仮想関数(オブジェクト描画関数)
	/// </summary>
	virtual void Draw() = 0;

protected:
	unique_ptr<Model> m_modelHandle;	//3Dモデルハンドル
	Vector3 m_pos;						//オブジェクトの座標
};

