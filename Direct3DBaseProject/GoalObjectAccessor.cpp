#include "pch.h"
#include"Json.h"
#include"GoalObject.h"
#include "GoalObjectAccessor.h"

GoalObjectAccessor* GoalObjectAccessor::m_instance = nullptr;

GoalObjectAccessor::GoalObjectAccessor()
{
	auto json = Json::GetInstance();
	m_goalObject = new GoalObject(json->Widen(json->GetData()["GoalBox"]).c_str(),
		Vector3(json->GetData()["PlayerClearPosition"].at(0),
			json->GetData()["PlayerClearPosition"].at(1),
			json->GetData()["PlayerClearPosition"].at(2)),
		float(json->GetData()["GoalBoxRotate"]));
}

GoalObjectAccessor::~GoalObjectAccessor()
{
	delete(m_goalObject);
}

void GoalObjectAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new GoalObjectAccessor();
}

void GoalObjectAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}