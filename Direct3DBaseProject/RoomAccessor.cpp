#include "pch.h"
#include"Json.h"
#include"Room.h"
#include "RoomAccessor.h"

RoomAccessor* RoomAccessor::m_instance = nullptr;

RoomAccessor::RoomAccessor()
{
	m_room = new Room(Json::GetInstance()->Widen(Json::GetInstance()->GetData()["Room"].at(0)).c_str());
}

RoomAccessor::~RoomAccessor()
{

}

void RoomAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new RoomAccessor();
}

void RoomAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}