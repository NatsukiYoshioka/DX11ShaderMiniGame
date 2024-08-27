#pragma once
#include"GameObject.h"

class GameObject;

class UIBase:public GameObject
{
public:
	UIBase(){}
	~UIBase(){}

	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:
	
};

