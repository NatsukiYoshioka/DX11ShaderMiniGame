#include "pch.h"
#include"UIBase.h"
#include"FoundUI.h"
#include "UIAccessor.h"

UIAccessor* UIAccessor::m_instance = nullptr;

UIAccessor::UIAccessor()
{
	m_UIs.push_back(new FoundUI());
}

UIAccessor::~UIAccessor()
{
	m_UIs.clear();
}

void UIAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new UIAccessor();
}

void UIAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}