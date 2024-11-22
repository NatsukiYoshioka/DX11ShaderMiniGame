#include "pch.h"
#include"Json.h"
#include"GoalObject.h"
#include "GoalObjectAccessor.h"

GoalObjectAccessor* GoalObjectAccessor::m_instance = nullptr;

//�I�u�W�F�N�g�̐���
GoalObjectAccessor::GoalObjectAccessor()
{
	auto json = Json::GetInstance();
	m_goalObject = new GoalObject(json->Widen(json->GetData()["GoalBox"]).c_str(),
		Vector3(json->GetData()["PlayerClearPosition"].at(0),
			json->GetData()["PlayerClearPosition"].at(1),
			json->GetData()["PlayerClearPosition"].at(2)),
		float(json->GetData()["GoalBoxRotate"]));
}

//�I�u�W�F�N�g�j��
GoalObjectAccessor::~GoalObjectAccessor()
{
	delete(m_goalObject);
}

//�C���X�^���X����
void GoalObjectAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new GoalObjectAccessor();
}

//�C���X�^���X�j��
void GoalObjectAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}