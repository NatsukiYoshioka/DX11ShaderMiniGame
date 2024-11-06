#pragma once

/// <summary>
/// �V�[���̊��N���X
/// </summary>
class BaseScene abstract
{
public:
	//�������z�֐�
	virtual void Update() abstract;
	virtual void Draw() abstract;
	virtual void DrawOffScreen() abstract;

	/// <summary>
	/// �V�[�����J�ڂ��Ă������ǂ����擾
	/// </summary>
	bool GetIsChangeScene()const { return m_isChangeScene; }
protected:
	bool m_isChangeScene = false;		//�V�[�����J�ڂ��Ă������ǂ���
};