#pragma once

class BaseScene;

/// <summary>
/// �V�[���Ǘ��N���X
/// </summary>
class SceneManager
{
private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SceneManager();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
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

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// �C���X�^���X�j��
	/// </summary>
	static void DestroyInstance();

	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	static SceneManager* GetInstance() { return m_instance; }

	/// <summary>
	/// �V�[���J�ڏ���
	/// </summary>
	static void ChangeScene(Scene scene);

	/// <summary>
	/// �V�[���X�V
	/// </summary>
	static void Update();

	/// <summary>
	/// �V�[���`��
	/// </summary>
	static void Draw();

	/// <summary>
	/// �I�t�X�N���[���`��
	/// </summary>
	static void DrawOffScreen();

	/// <summary>
	/// ���݂̃V�[�����擾
	/// </summary>
	static BaseScene* GetNowScene() { return m_nowScene; }

private:
	static SceneManager* m_instance;		//�}�l�[�W���[�̃C���X�^���X
	static BaseScene* m_nowScene;			//�V�[���̃C���X�^���X
};

