#include "pch.h"
#include"Json.h"
#include"DeviceAccessor.h"
#include"Block.h"
#include<WinBase.h>
#include<random>
#define _USE_MATH_DEFINES
#include<math.h>
#include "BlockAccessor.h"

BlockAccessor* BlockAccessor::m_instance = nullptr;

//ブロックの生成
BlockAccessor::BlockAccessor()
{
	auto json = Json::GetInstance();
	auto deviceAccessor = DeviceAccessor::GetInstance();
	vector<Vector3> pos;
	vector<float> rotate;
	vector<int> textureID;
	
	for (int i = 0; i < json->GetData()["Block"].size(); i++)
	{
		SetCurrentDirectory(L"Assets/Models");
		m_model.push_back(
			Model::CreateFromSDKMESH(
				deviceAccessor->GetDevice(),
				json->Widen(json->GetData()["Block"].at(i).at(0)).c_str(),
				*deviceAccessor->GetEffectFactory(),
				ModelLoader_Clockwise));
		SetCurrentDirectory(L"../../");
		pos.clear();
		rotate.clear();
		textureID.clear();
		for (int j = 1; j < json->GetData()["Block"].at(i).size(); j++)
		{
			pos.push_back(Vector3(json->GetData()["Block"].at(i).at(j).at(0),
				json->GetData()["Block"].at(i).at(j).at(1),
				json->GetData()["Block"].at(i).at(j).at(2)));
			rotate.push_back(float(json->GetData()["Block"].at(i).at(j).at(3)));
			std::random_device random;
			std::mt19937 generator(random());
			using dist_type = std::uniform_int_distribution<int>;
			dist_type distribution(0, json->GetData()["BlockTexture"].size() - 1);
			textureID.push_back(distribution(generator));
			/*m_blocks.push_back(new Block(
				json->Widen(json->GetData()["Block"].at(i).at(0)).c_str(),
				Vector3(json->GetData()["Block"].at(i).at(j).at(0),
					json->GetData()["Block"].at(i).at(j).at(1),
					json->GetData()["Block"].at(i).at(j).at(2)),
				float(json->GetData()["Block"].at(i).at(j).at(3))));*/
		}
		m_blocks.push_back(new Block(m_model.at(i).get(), pos, rotate, textureID));
	}
}

//ブロックの破棄
BlockAccessor::~BlockAccessor()
{
	m_blocks.clear();
	m_model.clear();
}

//インスタンス生成
void BlockAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new BlockAccessor();
}

//インスタンス破棄
void BlockAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}