#pragma once

class Enemy;

class EnemyAccessor
{
private:
	EnemyAccessor();
	~EnemyAccessor();
public:
	static void CreateInstance();
	static void DestroyInstance();
	static EnemyAccessor* GetInstance() { return m_instance; }

	Enemy* GetEnemy() { return m_enemy; }
private:
	static EnemyAccessor* m_instance;
	Enemy* m_enemy;
};

