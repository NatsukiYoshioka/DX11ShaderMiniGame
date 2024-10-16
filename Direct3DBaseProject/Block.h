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
	Block(Model* modelHandle, vector<Vector3> pos, vector<float> rotate, vector<int> textureID);

	/// <summary>
	/// データ破棄
	/// </summary>
	~Block();

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
	void DrawShadow();

	/// <summary>
	/// 敵視点での描画
	/// </summary>
	void DrawHitCheck();

	/// <summary>
	/// 頂点座標の取得
	/// </summary>
	/// <returns></returns>
	vector<XMFLOAT3> GetVertices() { return m_vertices; }

	/// <summary>
	/// 頂点座標のSRV取得
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D11ShaderResourceView> GetVertexBufferSRV() { return m_vertexBufferSRV; }

private:
	Model* m_model;								//モデル情報が格納されたクラスポインタ

	/// <summary>
	/// 描画時のインスタンシングに必要な変数の構造体
	/// </summary>
	struct InstanceData
	{
		XMMATRIX world;			//オブジェクトのワールド座標
		int textureID;			//オブジェクトのテクスチャID
	};
	vector<InstanceData> m_instances;			//インスタンシング構造体のコンテナ
	ComPtr<ID3D11Buffer> m_instanceBuffer;		//インスタンシング構造体バッファ
	shared_ptr<OriginalEffect> m_effect;		//モデル描画用エフェクトクラス
	ComPtr<ID3D11InputLayout> m_inputLayout;

	float m_rotate;		//モデルのY軸回転量
	Matrix m_world;		//モデルのワールド座標行列

	vector<XMFLOAT3> m_vertices;		//モデルの頂点座標
	ComPtr<ID3D11ShaderResourceView> m_vertexBufferSRV;		//頂点バッファSRV
	ComPtr<ID3D11UnorderedAccessView> m_outputBufferUAV;	//出力バッファUAV
};

