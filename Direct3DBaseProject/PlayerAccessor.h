#pragma once

class Player;

class PlayerAccessor
{
private:
	PlayerAccessor();
	~PlayerAccessor();
public:
	static void CreateInstance();
	static void DestroyInstance();
	static PlayerAccessor* GetInstance() { return m_instance; }

	Player* GetPlayer() { return m_player; }
private:
	static PlayerAccessor* m_instance;
	Player* m_player;
};

