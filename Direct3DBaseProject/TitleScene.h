#pragma once
#include"BaseScene.h"

class BaseScene;

/// <summary>
/// �^�C�g���V�[���Ǘ��N���X
/// </summary>
class TitleScene:public BaseScene
{
public:
	/// <summary>
	/// �^�C�g���V�[��������
	/// </summary>
	TitleScene();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~TitleScene();

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
	
	/// <summary>
	/// �Q�[�����X�^�[�g�����邩�ǂ����擾
	/// </summary>
	bool GetIsStartGame()const { return m_isStartGame; }
private:
	bool m_isStartGame;		//�Q�[�����X�^�[�g�����邩�ǂ���
};

