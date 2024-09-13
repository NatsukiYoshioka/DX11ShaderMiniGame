#pragma once

class Room;

class RoomAccessor
{
private:
	RoomAccessor();
	~RoomAccessor();
public:
	static void CreateInstance();
	static void DestroyInstance();
	static RoomAccessor* GetInstance() { return m_instance; }

	Room* GetRoom() { return m_room; }

private:
	static RoomAccessor* m_instance;
	Room* m_room;
};

