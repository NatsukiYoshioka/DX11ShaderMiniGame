#include "pch.h"
#include"SoundBase.h"
#include"TitleBGM.h"
#include"GameBGM.h"
#include"WalkSound.h"
#include"RunSound.h"
#include"FoundSE.h"
#include"GameClearBGM.h"
#include"GameOverBGM.h"
#include "SoundAccessor.h"

SoundAccessor* SoundAccessor::m_instance = nullptr;

SoundAccessor::SoundAccessor()
{
	m_sounds.push_back(new TitleBGM());
	m_sounds.push_back(new GameBGM());
	m_sounds.push_back(new WalkSound());
	m_sounds.push_back(new RunSound());
	m_sounds.push_back(new FoundSE());
	m_sounds.push_back(new GameClearBGM());
	m_sounds.push_back(new GameOverBGM());
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