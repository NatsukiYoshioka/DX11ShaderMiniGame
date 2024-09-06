#pragma once
using namespace std;

class UIBase;

/// <summary>
/// UI�p�A�N�Z�T�N���X
/// </summary>
class UIAccessor
{
private:
	/// <summary>
	/// UI�̐���
	/// </summary>
	UIAccessor();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~UIAccessor();
public:
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
	/// <returns></returns>
	static UIAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// UI�̎擾
	/// </summary>
	/// <returns></returns>
	vector<UIBase*> GetUIs() { return m_UIs; }
private:
	static UIAccessor* m_instance;		//�A�N�Z�T�̃C���X�^���X
	vector<UIBase*> m_UIs;				//UI�̃C���X�^���X�z��
};

