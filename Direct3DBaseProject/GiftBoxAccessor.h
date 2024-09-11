#pragma once

class GiftBox;

class GiftBoxAccessor
{
private:
	GiftBoxAccessor();
	~GiftBoxAccessor();
public:
	static void CreateInstance();
	static void DestroyInstance();
	static GiftBoxAccessor* GetInstance() { return m_instance; }

	GiftBox* GetGiftBox() { return m_giftBox; }

private:
	static GiftBoxAccessor* m_instance;
	GiftBox* m_giftBox;
};

