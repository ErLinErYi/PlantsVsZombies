/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#include "LZTestingGroundScene.h"
#include "LZTGBackgoundLayer.h"
#include "LZTGControlLayer.h"
#include "LZTGAnimationLayer.h"
#include "LZTGInformationLayer.h"
#include "LZTGButtonLayer.h"
#include "LZTGGameTimerLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZMouseEventControl.h"

TestingGroundScene::TestingGroundScene()
{
}

TestingGroundScene::~TestingGroundScene()
{
}

bool TestingGroundScene::init()
{
	if (!Scene::init())return false;

	showGameLayer();
	pauseGame();

	isRunGameScene = true;
	return true;
}

void TestingGroundScene::showGameLayer()
{
	controlPlayMusic();
	backgroundLayer();   // 背景层
	informationLayer();  // 信息层
	buttonLayer();       // 按钮层
	controlLayer();      // 控制层
	animationLayer();    // 动画层
	goodsLayer();        // 物品层
	gameTimerLayer();    // 时间层
	setName("GameScene");
}

void TestingGroundScene::controlPlayMusic()
{
	PlayMusic::changeBgMusic("bgMusic" + to_string(rand() % 14), true);
}

void TestingGroundScene::backgroundLayer()
{
	backgroundLayerInformation = TGBackgroundLayer::create();
	backgroundLayerInformation->addLayer(this, 0, "backgroundLayer");
}

void TestingGroundScene::controlLayer()
{
	controlLayerInformation = TGControlLayer::create();
	controlLayerInformation->addLayer(this, 1, "controlLayer");
}

void TestingGroundScene::animationLayer()
{
	animationLayerInformation = TGAnimationLayer::create(this);
	animationLayerInformation->addLayer(this, 2, "animationLayer");
}

void TestingGroundScene::informationLayer()
{
	informationLayerInformation = TGInformationLayer::create();
	informationLayerInformation->addLayer(this, 3, "informationLayer");
}

void TestingGroundScene::buttonLayer()
{
	buttonLayerInformation = TGButtonLayer::create();
	buttonLayerInformation->addLayer(this, 4, "buttonLayer");
}

void TestingGroundScene::goodsLayer()
{
	goodsLayerInformation = Layer::create();
	this->addChild(goodsLayerInformation, 5, "goodsLayer");
	MouseEventControl::goodsRecovery(goodsLayerInformation);
}

void TestingGroundScene::gameTimerLayer()
{
	addChild(TGGameTimerLayer::create(), 6, "gameTimerLayer");
}