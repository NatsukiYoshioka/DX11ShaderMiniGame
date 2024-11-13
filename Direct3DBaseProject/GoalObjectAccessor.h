#pragma once

class GoalObject;

class GoalObjectAccessor
{
private:
	GoalObjectAccessor();
	~GoalObjectAccessor();
public:
	static void CreateInstance();
	static void DestroyInstance();
	static GoalObjectAccessor* GetInstance() { return m_instance; }

	GoalObject* GetGoalObject() { return m_goalObject; }
private:
	static GoalObjectAccessor* m_instance;
	GoalObject* m_goalObject;
};

