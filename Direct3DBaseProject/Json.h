#pragma once
#include"json.hpp"

using json = nlohmann::json;
using namespace std;

class Json
{
private:
	Json();
	~Json();
public:
	static void CreateInstance();
	static void DestroyInstance();
	static Json* GetInstance() { return m_instance; }
	json GetData() { return m_json; }
private:
	static Json* m_instance;

	static const string m_filePath;
	json m_json;
};

