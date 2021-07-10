/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.02
 *Email: 2117610943@qq.com
 */
#include "LZIZombiesScene.h"
#include "LZIBackGroundLayer.h"
#include "LZIInformationLayer.h"
#include "LZIButtonLayer.h"
#include "LZIAnimationLayer.h"
#include "LZIControlLayer.h"
#include "LZIOpenCaveGameLayer.h"
#include "LZIGameTimerLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseQuitLayer.h"
#include "Based/LZPlayMusic.h"

IZombiesScene::IZombiesScene()
{
}

IZombiesScene::~IZombiesScene()
{
	IOpenCaveGameLayer::isReadFile = false;
}

bool IZombiesScene::init()
{
	if (!Scene::init())return false;

	backgroundLayer();   // 背景层
	informationLayer();  // 信息层
	buttonLayer();       // 按钮层
	
	runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]() {isRunGameScene = true; }), nullptr));

	return true;
}

void IZombiesScene::onEnterTransitionDidFinish()
{
	animationLayer();    // 动画层
	controlLayer();      // 控制层
	goodsLayer();        // 物品层
	gameTimerLayer();    // 定时层
	controlPlayMusic();
	runAction(Sequence::create(DelayTime::create(1.f), CallFunc::create([&]() {pauseGame(); }), nullptr));

	setName("GameScene");
}

void IZombiesScene::controlPlayMusic()
{
	PlayMusic::changeBgMusic("mainmusic.mo3.7", true);
}

void IZombiesScene::backgroundLayer()
{
	backgroundLayerInformation = IBackgroundLayer::create();
	backgroundLayerInformation->addLayer(this, 0, "backgroundLayer");
}

void IZombiesScene::informationLayer()
{
	informationLayerInformation = IInformationLayer::create();
	informationLayerInformation->addLayer(this, 4, "informationLayer");
}

void IZombiesScene::buttonLayer()
{
	buttonLayerInformation = IButtonLayer::create();
	buttonLayerInformation->addLayer(this, 3, "buttonLayer");
}

void IZombiesScene::controlLayer()
{
	controlLayerInformation = IControlLayer::create();
	controlLayerInformation->addLayer(this, 1, "controlLayer");
}

void IZombiesScene::animationLayer()
{
	animationLayerInformation = IAnimationLayer::create();
	animationLayerInformation->addLayer(this, 2, "animationLayer");
}

void IZombiesScene::gameTimerLayer()
{
	this->addChild(IGameTimerLayer::create(), 6, "gameTimerLayer");
}