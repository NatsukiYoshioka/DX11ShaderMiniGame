#include "pch.h"
#include"UIBase.h"
#include"Transition.h"
#include"GameOverEffect.h"
#include"GameOverUI.h"
#include"GameClearUI.h"
#include"ControlUI.h"
#include"FoundEffect.h"
#include"FoundUI.h"
#include"ButtonA.h"
#include"TitleLogo.h"
#include "UIAccessor.h"

UIAccessor* UIAccessor::m_instance = nullptr;

//UI����
UIAccessor::UIAccessor()
{
	m_UIs.push_back(new TitleLogo());
	m_UIs.push_back(new ButtonA());
	m_UIs.push_back(new FoundUI());
	//m_UIs.push_back(new FoundEffect());
	m_UIs.push_back(new ControlUI());
	m_UIs.push_back(new GameClearUI());
	m_UIs.push_back(new GameOverUI());
	m_UIs.push_back(new GameOverEffect());
	m_UIs.push_back(new Transition());
}

//�f�[�^�j��
UIAccessor::~UIAccessor()
{
	m_UIs.clear();
}

//�C���X�^���X����
void UIAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new UIAccessor();
}

//�C���X�^���X�j��
void UIAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}