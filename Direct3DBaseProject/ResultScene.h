#pragma once

class BaseScene;

/// <summary>
/// ���U���g�V�[���Ǘ��N���X
/// </summary>
class ResultScene : public BaseScene
{
public:
	/// <summary>
	/// �V�[��������
	/// </summary>
	ResultScene();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~ResultScene();

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

