/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.04
 *Email: 2117610943@qq.com
 */
#include "LZIOpenCaveGameLayer.h"
#include "LZIPauseQuiteLayer.h"
#include "Scenes/GameScenes/IZombies/GameScene/LZIZombiesData.h"

bool IOpenCaveGameLayer::isReadFile = false;

IOpenCaveGameLayer::IOpenCaveGameLayer():
  _global(Global::getInstance())
, _director(Director::getInstance())
{
	_levelName = new char[32];
	strcpy(_levelName, "IZombiesData");
}

IOpenCaveGameLayer::~IOpenCaveGameLayer()
{
	isReadFile = false;
	if (_levelName)delete _levelName;
}

bool IOpenCaveGameLayer::init()
{
	if (!Scene::init())return false;

	isReadFile = true;
	
	setName("GameScene");

	return true;
}

void IOpenCaveGameLayer::onEnterTransitionDidFinish()
{
	backgroundLayer();   // 背景层
	informationLayer();  // 信息层
	buttonLayer();       // 按钮层
	animationLayer();    // 动画层
	controlLayer();      // 控制层
	goodsLayer();        // 物品层
	gameTimerLayer();    // 定时层
	controlPlayMusic();
	
	openLevelData();
	runAction(Sequence::create(DelayTime::create(1.3f), CallFunc::create([&]() {pauseGame(); }), nullptr));

	GSPauseQuitLayer::pauseLayer();
	_director->getRunningScene()->addChild(IPauseQuitLayer::create(), 10, "pauseLayer");

	auto layer = LayerColor::create(Color4B(0, 0, 0, 255));
	layer->setGlobalZOrder(2);
	this->addChild(layer);
	layer->runAction(Sequence::create(FadeOut::create(1.0f),
		CallFunc::create([=]()
			{
				layer->removeFromParent();
			}), nullptr));
}

void IOpenCaveGameLayer::openLevelData()
{
	IZombiesData::getInstance()->createNewLevelDataDocument();
	IZombiesData::getInstance()->openLevelData(_levelName);
}
