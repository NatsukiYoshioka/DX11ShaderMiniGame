#pragma once

class DollHead;

class DollHeadAccessor
{
private:
	DollHeadAccessor();
	~DollHeadAccessor();
public:
	static void CreateInstance();
	static void DestroyInstance();
	static DollHeadAccessor* GetInstance() { return m_instance; }

	DollHead* GetDollHead() { return m_dollHead; }
private:
	static DollHeadAccessor* m_instance;
	DollHead* m_dollHead;
};

