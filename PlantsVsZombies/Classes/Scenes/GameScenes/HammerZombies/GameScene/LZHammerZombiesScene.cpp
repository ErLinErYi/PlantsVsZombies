/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */

#include "LZHammerZombiesScene.h"
#include "LZHBackGroundLayer.h"
#include "LZHInformationLayer.h"
#include "LZHButtonLayer.h"
#include "LZHControlLayer.h"
#include "LZHAnimationLayer.h"
#include "LZHGameTimerLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseQuitLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Based/LZPlayMusic.h"

HammerZombiesScene::HammerZombiesScene()
{
}

HammerZombiesScene::~HammerZombiesScene()
{
	GSPauseQuitLayer::setPause(false);
}

bool HammerZombiesScene::init()
{
	if (!Scene::init())return false;

	backgroundLayer();   // 背景层
	informationLayer();  // 信息层
	buttonLayer();       // 按钮层
	
	setName("GameScene");

	return true;
}

void HammerZombiesScene::onEnterTransitionDidFinish()
{
	controlLayer();      // 控制层
	animationLayer();    // 动画层
	goodsLayer();        // 物品层
	gameTimerLayer();    // 定时层
	controlPlayMusic();
	runAction(Sequence::create(DelayTime::create(1.f), CallFunc::create([&]() {pauseGame(); }), nullptr));
}

void HammerZombiesScene::controlPlayMusic()
{
	PlayMusic::changeBgMusic("bgMusic2", true);
}

void HammerZombiesScene::backgroundLayer()
{
	backgroundLayerInformation = HBackgroundLayer::create();
	backgroundLayerInformation->addLayer(this, 0, "backgroundLayer");
}

void HammerZombiesScene::informationLayer()
{
	informationLayerInformation = HInformationLayer::create();
	informationLayerInformation->addLayer(this, 4, "informationLayer");
}

void HammerZombiesScene::buttonLayer()
{
	buttonLayerInformation = HButtonLayer::create();
	buttonLayerInformation->addLayer(this, 3, "buttonLayer");
}

void HammerZombiesScene::controlLayer()
{
	controlLayerInformation = HControlLayer::create();
	controlLayerInformation->addLayer(this, 1, "controlLayer");
}

void HammerZombiesScene::animationLayer()
{
	animationLayerInformation = HAnimationLayer::create();
	animationLayerInformation->addLayer(this, 2, "animationLayer");
}

void HammerZombiesScene::gameTimerLayer()
{
	this->addChild(HGameTimerLayer::create(), 6, "gameTimerLayer");
}

void HammerZombiesScene::goodsLayer()
{
	goodsLayerInformation = Layer::create();
	this->addChild(goodsLayerInformation, 5, "goodsLayer");
}
