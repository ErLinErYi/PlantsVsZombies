/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.5.03
 *Email: 2117610943@qq.com
 */

#include "OpenCaveGameScene.h"
#include "GSPauseQuitLayer.h"
#include "Based/UserData.h"
#include "GSButtonLayer.h"

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

	controlPlayMusic();
	backgroundLayer();   // ±³¾°²ã
	informationLayer();  // ÐÅÏ¢²ã
	buttonLayer();       // °´Å¥²ã
	controlLayer();      // ¿ØÖÆ²ã
	animationLayer();    // ¶¯»­²ã
	
	openLevelData();
	
#if CC_TARGET_PLATFORM ==CC_PLATFORM_WIN32
	schedule([&](float) {
		pauseGame();
		}, 1.0f, CC_REPEAT_FOREVER, 2.f, "pauseGame");
#endif
	return true;
}

void OpenCaveGameScene::onEnter()
{
	Scene::onEnter();

	runAction(Sequence::create(DelayTime::create(0.01f),
		CallFunc::create([=]()
			{
				GSPauseQuitLayer::pauseLayer();
				_director->getRunningScene()->addChild(GSPauseQuitLayer::addLayer(), 10, "pauseLayer");
			}), nullptr));

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
