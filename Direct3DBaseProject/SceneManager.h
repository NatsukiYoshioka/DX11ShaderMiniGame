#pragma once

class BaseScene;

class SceneManager
{
private:
	SceneManager();
	~SceneManager();
public:
	/// <summary>
	/// �V�[���̏�Ԃ��Ǘ�
	/// </summary>
	enum class Scene
	{
		Title,								//�^�C�g�����
		Game,								//�Q�[�����
		Result								//���U���g���
	};

	static void CreateInstance();
	static void DestroyInstance();
	static SceneManager* GetInstance() { return m_instance; }

	static void ChangeScene(Scene scene);

	static void Update();
	static void Draw();
	static void DrawOffScreen();

	static BaseScene* GetNowScene() { return m_nowScene; }

private:
	static SceneManager* m_instance;
	static BaseScene* m_nowScene;
};

