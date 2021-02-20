/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#include "LZAGameScene.h"
#include "LZAGSBackgroundLayer.h"
#include "LZAGSButtonLayer.h"
#include "LZAGSInformationLayer.h"
#include "LZAGSControlLayer.h"
#include "LZAGSAnimationLayer.h"
#include "LZAGSPauseQuitLayer.h"
#include "LZAGSPauseLayer.h"
#include "LZAGSGameTimerLayer.h"
#include "LZAGSDefine.h"
#include "LZAGSData.h"

#include "Based/LZCar.h"
#include "Based/LZCoin.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZUserData.h"
#include "Based/LZMouseEventControl.h"

#include "Plants/DefensePlants/LZSunFlower.h"
#include "Plants/EmissionPlants/Bullet/LZBullet.h"
#include "Zombies/LZZombies.h"

bool GameScene::isRunGameScene = false;

GameScene::GameScene() :
  _global(Global::getInstance())
, _director(Director::getInstance())
{
}

GameScene::~GameScene()
{
	DELETE_MAP_GROUP(PlantsGroup);
	DELETE_LIST_GROUP(ZombiesGroup);
	DELETE_LIST_GROUP(SunsGroup);
	DELETE_LIST_GROUP(BulletGroup);
	DELETE_LIST_GROUP(CarsGroup);
	DELETE_LIST_GROUP(CoinsGroup);

	Zombies::setZombiesNumbers(0);
	Zombies::zombiesWinOrLoseInit();

	GSPauseQuitLayer::setPause(false);

	_director->getEventDispatcher()->
		removeCustomEventListeners(GLViewImpl::EVENT_WINDOW_UNFOCUSED);

	isRunGameScene = false;
}

bool GameScene::init()
{
	if (!Scene::init())return false; 

	showGameLayer();
	pauseGame();

	isRunGameScene = true;
	return true;
}

void GameScene::showGameLayer()
{
	controlPlayMusic();
	backgroundLayer();   // 背景层
	informationLayer();  // 信息层
	buttonLayer();       // 按钮层
	controlLayer();      // 控制层
	animationLayer();    // 动画层
	goodsLayer();        // 物品层
	gameTimerLayer();    // 时间层
}

void GameScene::controlPlayMusic()
{
	switch (_global->userInformation->getCurrentPlayLevels())
	{
	case 35: PlayMusic::changeBgMusic("mainmusic.mo3.2", true);                      break;
	case 36: PlayMusic::changeBgMusic("mainmusic.mo3.3", true);                      break;
	case 37: PlayMusic::changeBgMusic("mainmusic.mo3.7", true);                      break;
	case 50: PlayMusic::changeBgMusic("mainmusic.mo3.10", true);                     break;
	case 51: PlayMusic::changeBgMusic("mainmusic.mo3.11", true);                     break;
	case 52: PlayMusic::changeBgMusic("mainmusic.mo3.12", true);                     break;
	default: PlayMusic::changeBgMusic("bgMusic" + to_string(rand() % 11), true);     break;
	}
}

void GameScene::backgroundLayer()
{
	backgroundLayerInformation = GSBackgroundLayer::create();
	backgroundLayerInformation->addLayer(this, 0, "backgroundLayer");
}

void GameScene::controlLayer()
{
	controlLayerInformation = GSControlLayer::create();
	controlLayerInformation->addLayer(this, 1, "controlLayer");
}

void GameScene::animationLayer()
{
	animationLayerInformation = GSAnimationLayer::create(this);
	animationLayerInformation->addLayer(this, 2, "animationLayer");
}

void GameScene::informationLayer()
{
	informationLayerInformation = GSInformationLayer::create();
	informationLayerInformation->addLayer(this, 3, "informationLayer");
}

void GameScene::buttonLayer()
{
	buttonLayerInformation = GSButtonLayer::create();
	buttonLayerInformation->addLayer(this, 4, "buttonLayer");
}

void GameScene::goodsLayer()
{
	goodsLayerInformation = Layer::create();
	this->addChild(goodsLayerInformation, 5, "goodsLayer");
	MouseEventControl::goodsRecovery(goodsLayerInformation);
}

void GameScene::gameTimerLayer()
{
	this->addChild(GSGameTimerLayer::create(), 6, "gameTimerLayer");
}

void GameScene::pauseGame()
{
	_director->getEventDispatcher()->addCustomEventListener(
		GLViewImpl::EVENT_WINDOW_UNFOCUSED, [&](EventCustom* evt)
		{
			if (!GSPauseQuitLayer::getIsPause())
			{
				PlayMusic::playMusic("pause");
				GSPauseQuitLayer::pauseLayer();
				_director->getRunningScene()->addChild(GSPauseLayer::addLayer(), 10);
			}
		});
}
