#pragma once
#include"GameObject.h"

class GameObject;

/// <summary>
/// UIの基底クラス
/// </summary>
class UIBase abstract :public GameObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	UIBase(){}

	/// <summary>
	/// デストラクタ
	/// </summary>
	~UIBase(){}

	//純粋仮想関数
	virtual void InitializeTitle() abstract;
	virtual void UpdateTitle() abstract;
	virtual void DrawTitle() abstract;
	virtual void Initialize() abstract;
	virtual void Update() abstract;
	virtual void Draw() abstract;
	virtual void InitializeResult() abstract;
	virtual void UpdateResult() abstract;
	virtual void DrawResult() abstract;
	void DrawShadow()override{}

protected:

};

