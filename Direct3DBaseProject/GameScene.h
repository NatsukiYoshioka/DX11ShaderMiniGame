#pragma once

class BaseScene;

/// <summary>
/// ���C���Q�[���V�[���Ǘ��N���X
/// </summary>
class GameScene : public BaseScene
{
public:
	/// <summary>
	/// �Q�[���V�[��������
	/// </summary>
	GameScene();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~GameScene();

	/// <summary>
	/// �V�[���X�V
	/// </summary>
	void Update()override;

	/// <summary>
	/// �V�[���`��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// �I�t�X�N���[���`��
	/// </summary>
	void DrawOffScreen()override;
};

