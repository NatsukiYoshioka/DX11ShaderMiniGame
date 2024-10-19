#pragma once
#include"GameObject.h"

class GameObject;

/// <summary>
/// UIの基底クラス
/// </summary>
class UIBase:public GameObject
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
	virtual void InitializeTitle() = 0;
	virtual void UpdateTitle() = 0;
	virtual void DrawTitle() = 0;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void InitializeResult() = 0;
	virtual void UpdateResult() = 0;
	virtual void DrawResult() = 0;
	void DrawShadow()override{}

protected:

};

