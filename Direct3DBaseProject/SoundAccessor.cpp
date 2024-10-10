#include "pch.h"
#include"SoundBase.h"
#include"TitleBGM.h"
#include "SoundAccessor.h"

SoundAccessor* SoundAccessor::m_instance = nullptr;

SoundAccessor::SoundAccessor()
{
	m_sounds.push_back(new TitleBGM());
}

SoundAccessor::~SoundAccessor()
{
	m_sounds.clear();
}

void SoundAccessor::CreateInstance()
{
	if (m_instance)return;
	m_instance = new SoundAccessor();
}

void SoundAccessor::DestroyInstance()
{
	if (!m_instance)return;
	delete(m_instance);
}