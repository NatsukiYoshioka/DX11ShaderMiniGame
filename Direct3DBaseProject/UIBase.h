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
	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:
	
};

