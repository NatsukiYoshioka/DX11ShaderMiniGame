#pragma once
#include"json.hpp"

using json = nlohmann::json;
using namespace std;

/// <summary>
/// Json�f�[�^�Ǘ��N���X
/// </summary>
class Json
{
private:
	/// <summary>
	/// �f�[�^�̃��[�h
	/// </summary>
	Json();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~Json();
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
	static Json* GetInstance() { return m_instance; }

	/// <summary>
	/// Json�f�[�^�擾
	/// </summary>
	/// <returns></returns>
	json GetData() { return m_json; }

	/// <summary>
	/// string�^��wstring�^�ϊ�
	/// </summary>
	/// <param name="str">�ϊ����镶����</param>
	/// <returns>�ϊ�����������</returns>
	wstring Widen(const string& str);
private:
	static Json* m_instance;		//�N���X�̃C���X�^���X

	static const string m_filePath;	//Json�t�@�C���̃p�X
	json m_json;					//Json�̃f�[�^
};

