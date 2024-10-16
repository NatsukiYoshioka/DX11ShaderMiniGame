#pragma once

class Room;

class RoomAccessor
{
private:
	/// <summary>
	/// ���f���̐���
	/// </summary>
	RoomAccessor();

	/// <summary>
	/// ���f���̔j��
	/// </summary>
	~RoomAccessor();
public:
	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// �C���X�^���X�̔j��
	/// </summary>
	static void DestroyInstance();

	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns></returns>
	static RoomAccessor* GetInstance() { return m_instance; }

	/// <summary>
	/// �I�u�W�F�N�g�̃C���X�^���X�擾
	/// </summary>
	/// <returns></returns>
	Room* GetRoom() { return m_room; }

private:
	static RoomAccessor* m_instance;	//�A�N�Z�T�̃C���X�^���X
	Room* m_room;						//�I�u�W�F�N�g�̃C���X�^���X
};

