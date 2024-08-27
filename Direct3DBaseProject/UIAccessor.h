#pragma once
using namespace std;

class UIBase;

class UIAccessor
{
private:
	UIAccessor();
	~UIAccessor();
public:
	static void CreateInstance();
	static void DestroyInstance();
	static UIAccessor* GetInstance() { return m_instance; }
	vector<UIBase*> GetUIs() { return m_UIs; }
private:
	static UIAccessor* m_instance;
	vector<UIBase*> m_UIs;
};

