/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#include "LZTGPauseQuitLayer.h"
#include "../SelectPlantsScene/LZTSelectPlantsScene.h"
#include "Scenes/MainMenuScene/LZMainMenu.h"

TGPauseQuitLayer::TGPauseQuitLayer()
{
}

TGPauseQuitLayer::~TGPauseQuitLayer()
{
}

bool TGPauseQuitLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createDialog();

	return true;
}

void TGPauseQuitLayer::setRestart()
{
	_director->getScheduler()->setTimeScale(1.0f);
	_director->replaceScene(TransitionFade::create(1.0f, TSelectPlantsScene::create()));
}

void TGPauseQuitLayer::setQuitGame()
{
	_director->getScheduler()->setTimeScale(1.0f);
	popSceneAnimation();
}

void TGPauseQuitLayer::popSceneAnimation()
{
	_director->replaceScene(TransitionFade::create(0.5f, MainMenu::create()));
}
