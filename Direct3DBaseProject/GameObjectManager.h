#pragma once
using namespace std;

class GameObject;

class GameObjectManager
{
private:
	GameObjectManager();
	~GameObjectManager();
public:
	static void CreateInstance();
	static void DestroyInstance();
	static GameObjectManager* GetInstance() { return m_instance; }

	void Update();
	void Draw();

private:
	static GameObjectManager* m_instance;

	vector<GameObject*> m_gameObjects;
};

