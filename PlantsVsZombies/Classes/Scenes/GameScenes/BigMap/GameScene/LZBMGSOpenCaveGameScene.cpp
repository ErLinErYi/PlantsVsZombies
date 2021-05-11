/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.14
 *Email: 2117610943@qq.com
 */

#include "LZBMGSOpenCaveGameScene.h"
#include "LZBMGSPauseQuitLayer.h"
#include "Based/LZUserData.h"

BMOpenCaveGameScene::BMOpenCaveGameScene():
	_levelName(Global::getInstance()->userInformation->getCurrentCaveFileLevelWorldName())
{
}

BMOpenCaveGameScene::~BMOpenCaveGameScene()
{
	_global->userInformation->setIsReadFileData(false);
}

bool BMOpenCaveGameScene::init()
{
	if (!Scene::init())return false;

	UserData::getInstance()->createNewLevelDataDocument();
	UserData::getInstance()->openLevelSelectCardData(_levelName);

	showGameLayer();

	openLevelData();

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	pauseGame();
#endif
	return true;
}

void BMOpenCaveGameScene::onEnter()
{
	Scene::onEnter();

	GSPauseQuitLayer::pauseLayer();
	_director->getRunningScene()->addChild(BMPauseQuitLayer::create(), 10, "pauseLayer");

	auto layer = LayerColor::create(Color4B(0, 0, 0, 255));
	layer->setGlobalZOrder(2);
	this->addChild(layer);
	layer->runAction(Sequence::create(FadeOut::create(1.0f),
		CallFunc::create([=]()
			{
				layer->removeFromParent();
			}), nullptr));
}

void BMOpenCaveGameScene::openLevelData()
{
	UserData::getInstance()->openLevelData(_levelName);
}
