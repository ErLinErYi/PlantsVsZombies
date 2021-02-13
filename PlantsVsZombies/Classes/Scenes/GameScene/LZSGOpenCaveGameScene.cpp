/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.5.03
 *Email: 2117610943@qq.com
 */

#include "LZSGOpenCaveGameScene.h"
#include "LZSGPauseQuitLayer.h"
#include "Based/LZBUserData.h"
#include "LZSGButtonLayer.h"

OpenCaveGameScene::OpenCaveGameScene() :
	_global(Global::getInstance())
,   _director(Director::getInstance())
,   _levelName(Global::getInstance()->userInformation->getCurrentCaveFileLevelWorldName())
{
}

OpenCaveGameScene::~OpenCaveGameScene()
{
	_global->userInformation->setIsReadFileData(false);
}

Scene* OpenCaveGameScene::createScene()
{
	return OpenCaveGameScene::create();
}

bool OpenCaveGameScene::init()
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

void OpenCaveGameScene::onEnter()
{
	Scene::onEnter();

	GSPauseQuitLayer::pauseLayer();
	_director->getRunningScene()->addChild(GSPauseQuitLayer::addLayer(), 10, "pauseLayer");
	
	auto layer = LayerColor::create(Color4B(0, 0, 0, 255));
	layer->setGlobalZOrder(2);
	this->addChild(layer);
	layer->runAction(Sequence::create(FadeOut::create(1.0f),
		CallFunc::create([=]()
			{
				layer->removeFromParent();
			}), nullptr));
}

void OpenCaveGameScene::openLevelData()
{
	UserData::getInstance()->openLevelData(_levelName);
}
