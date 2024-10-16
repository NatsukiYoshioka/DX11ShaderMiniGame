#include "pch.h"
#include"Json.h"
#include"Room.h"
#include "RoomAccessor.h"

RoomAccessor* RoomAccessor::m_instance = nullptr;

//モデルの生成
RoomAccessor::RoomAccessor()
{
	m_room = new Room(Json::GetInstance()->Widen(Json::GetInstance()->GetData()["Room"].at(0)).c_str());
}

//データ破棄
RoomAccessor::~RoomAccessor()
{

}

//インスタンスの生成
void RoomAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new RoomAccessor();
}

//インスタンス破棄
void RoomAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}