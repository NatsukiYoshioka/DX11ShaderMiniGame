#pragma once
#include"GameObject.h"
using namespace DirectX;
using namespace SimpleMath;
using namespace Microsoft::WRL;
using namespace std;

class GameObject;
class OriginalEffect;

/// <summary>
/// 積み木ブロックオブジェクト
/// </summary>
class Block:public GameObject
{
public:
	/// <summary>
	/// 積み木ブロックの初期化
	/// </summary>
	/// <param name="fileName">3Dモデルのファイル名</param>
	/// <param name="pos">オブジェクトの初期座標</param>
	/// <param name="rotate">オブジェクトのY軸回転量(XM_PI / rotate)</param>
	Block(const wchar_t* fileName, Vector3 pos, float rotate);

	/// <summary>
	/// データ破棄
	/// </summary>
	~Block();

	/// <summary>
	/// オブジェクトの更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// オブジェクトの描画
	/// </summary>
	void Draw()override;

private:
	shared_ptr<OriginalEffect> m_effect;		//モデル描画用エフェクトクラス

	float m_rotate;		//モデルのY軸回転量
	Matrix m_world;		//モデルのワールド座標行列
};

