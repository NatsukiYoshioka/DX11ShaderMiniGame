#pragma once

class GoalObject;

class GoalObjectAccessor
{
private:
	/// <summary>
	/// �I�u�W�F�N�g�̐���
	/// </summary>
	GoalObjectAccessor();

	/// <summary>
	/// �I�u�W�F�N�g�j��
	/// </summary>
	~GoalObjectAccessor();
public:
	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// �C���X�^���X�j��
	/// </summary>
	static void DestroyInstance();

	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	static GoalObjectAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// �I�u�W�F�N�g�擾
	/// </summary>
	GoalObject* GetGoalObject() { return m_goalObject; }
private:
	static GoalObjectAccessor* m_instance;		//�A�N�Z�T�̃C���X�^���X
	GoalObject* m_goalObject;					//�I�u�W�F�N�g�̃C���X�^���X
};

