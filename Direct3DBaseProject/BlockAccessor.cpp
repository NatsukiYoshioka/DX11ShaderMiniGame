#include "pch.h"
#include"Json.h"
#include"Block.h"
#include "BlockAccessor.h"

BlockAccessor* BlockAccessor::m_instance = nullptr;

//�u���b�N�̐���
BlockAccessor::BlockAccessor()
{
	auto json = Json::GetInstance();
	for (int i = 0; i < json->GetData()["Block"].size(); i++)
	{
		for (int j = 1; j < json->GetData()["Block"].at(i).size(); j++)
		{
			m_blocks.push_back(new Block(
				json->Widen(json->GetData()["Block"].at(i).at(0)).c_str(),
				Vector3(json->GetData()["Block"].at(i).at(j).at(0),
					json->GetData()["Block"].at(i).at(j).at(1),
					json->GetData()["Block"].at(i).at(j).at(2)),
				float(json->GetData()["Block"].at(i).at(j).at(3))));
		}
	}
}

//�u���b�N�̔j��
BlockAccessor::~BlockAccessor()
{
	m_blocks.clear();
}

//�C���X�^���X����
void BlockAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new BlockAccessor();
}

//�C���X�^���X�j��
void BlockAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}