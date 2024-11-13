#include "pch.h"
#include"GoalObject.h"
#include "GoalObjectAccessor.h"

GoalObjectAccessor* GoalObjectAccessor::m_instance = nullptr;

GoalObjectAccessor::GoalObjectAccessor()
{
	m_goalObject = new GoalObject();
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