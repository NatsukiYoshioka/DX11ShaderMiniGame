#include "pch.h"
#include "BaseScene.h"
#include"GameObjectManager.h"
#include "ResultScene.h"

ResultScene::ResultScene()
{
	auto gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->InitializeResult();
}

ResultScene::~ResultScene()
{

}

void ResultScene::Update()
{
	auto gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->UpdateResult();
}

void ResultScene::Draw()
{
	auto gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->DrawResult();
}

void ResultScene::DrawOffScreen()
{

}