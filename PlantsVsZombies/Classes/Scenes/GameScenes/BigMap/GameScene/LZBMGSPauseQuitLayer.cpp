/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.15
 *Email: 2117610943@qq.com
 */

#include "LZBMGSPauseQuitLayer.h"
#include "Based/LZUserData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSGameEndLayer.h"
#include "Scenes/GameScenes/BigMap/SelectPlantsScene/LZBMSelectPlantsScene.h"

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
	CREATEDIALOG(
	    _director->getScheduler()->setTimeScale(1.0f);
	    UserData::getInstance()->caveUserData(const_cast<char*>("BREAKTHROUGH"), ++_global->userInformation->getBreakThroughNumbers());
	    GSGameEndLayer::judgeBreakThroughAboutJumpLevel();

		_global->userInformation->setDynamicDifficultyValue(_global->userInformation->getDynamicDifficultyValue() - 10 - rand() % 10);
		UserData::getInstance()->caveUserData(const_cast<char*>("DYNAMICDIFFICULTYVALUE"), _global->userInformation->getDynamicDifficultyValue());

	    _director->replaceScene(TransitionFade::create(1.0f, BMSelectPlantsScene::create()));

	    UserData::getInstance()->createNewLevelDataDocument();
	    UserData::getInstance()->removeLevelData(_levelName);
	)
}
