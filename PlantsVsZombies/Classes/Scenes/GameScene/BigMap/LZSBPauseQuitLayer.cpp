/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.15
 *Email: 2117610943@qq.com
 */

#include "LZSBPauseQuitLayer.h"
#include "Based/LZBUserData.h"
#include "../LZSGGameEndLayer.h"
#include "../../SelectPlantsScene/BigMap/LZSBSelectPlantsScene.h"

BMPauseQuitLayer::BMPauseQuitLayer()
{
	_levelName = _global->userInformation->getCurrentCaveFileLevelWorldName();
}

BMPauseQuitLayer::~BMPauseQuitLayer()
{
}

bool BMPauseQuitLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createDialog();

	return true;
}

void BMPauseQuitLayer::setRestart()
{
	_director->getScheduler()->setTimeScale(1.0f);
	UserData::getInstance()->caveUserData("BREAKTHROUGH", ++_global->userInformation->getBreakThroughNumbers());
	GSGameEndLayer::judgeBreakThroughAboutJumpLevel();

	_director->replaceScene(TransitionFade::create(1.0f, BSelectPlantsScene::create()));

	UserData::getInstance()->createNewLevelDataDocument();
	UserData::getInstance()->removeLevelData(_levelName);
}
