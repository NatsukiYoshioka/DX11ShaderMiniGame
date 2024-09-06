#include "pch.h"
#include"UIBase.h"
#include"FoundUI.h"
#include "UIAccessor.h"

UIAccessor* UIAccessor::m_instance = nullptr;

//UI生成
UIAccessor::UIAccessor()
{
	m_UIs.push_back(new FoundUI());
}

//データ破棄
UIAccessor::~UIAccessor()
{
	m_UIs.clear();
}

//インスタンス生成
void UIAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new UIAccessor();
}

//インスタンス破棄
void UIAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}