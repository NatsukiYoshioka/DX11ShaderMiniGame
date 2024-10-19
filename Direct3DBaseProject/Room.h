#pragma once
#include"GameObject.h"

using namespace std;
using namespace Microsoft::WRL;

class GameObject;
class OriginalEffect;

class Room:public GameObject
{
public:
	/// <summary>
	/// オブジェクトの初期化
	/// </summary>
	/// <param name="fileName"></param>
	Room(const wchar_t* fileName);

	/// <summary>
	/// データ破棄
	/// </summary>
	~Room();

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
	vector<ComPtr<ID3D11ShaderResourceView>> texture;		//テクスチャのコンテナ
	vector<shared_ptr<OriginalEffect>> m_effect;			//シェーダー用クラス

	Matrix m_world;		//ワールド行列
	float m_rotate;		//オブジェクトの回転量
};

