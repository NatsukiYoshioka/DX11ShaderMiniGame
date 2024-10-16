#pragma once
using namespace std;

class SoundBase;

class SoundAccessor
{
private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SoundAccessor();

	/// <summary>
	/// �f�[�^�j��
	/// </summary>
	~SoundAccessor();

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
	static SoundAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// �T�E���h�I�u�W�F�N�g�̎擾
	/// </summary>
	/// <returns></returns>
	vector<SoundBase*> GetSounds() { return m_sounds; }
private:
	static SoundAccessor* m_instance;		//�A�N�Z�T�̃C���X�^���X
	vector<SoundBase*> m_sounds;			//�T�E���h�I�u�W�F�N�g�̃R���e�i
};

