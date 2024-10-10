#pragma once
using namespace std;

class SoundBase;

class SoundAccessor
{
private:
	SoundAccessor();
	~SoundAccessor();

public:
	static void CreateInstance();
	static void DestroyInstance();
	static SoundAccessor* GetInstance() { return m_instance; }
	vector<SoundBase*> GetSounds() { return m_sounds; }
private:
	static SoundAccessor* m_instance;
	vector<SoundBase*> m_sounds;
};

