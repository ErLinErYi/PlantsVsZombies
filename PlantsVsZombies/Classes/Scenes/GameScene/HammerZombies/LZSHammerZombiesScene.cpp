/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */

#include "LZSHammerZombiesScene.h"
#include "LZSHBackGroundLayer.h"
#include "LZSHInformationLayer.h"
#include "LZSHButtonLayer.h"
#include "LZSHControlLayer.h"
#include "LZSHAnimationLayer.h"
#include "LZSHGameTimerLayer.h"
#include "../LZSGPauseQuitLayer.h"
#include "../LZSGPauseLayer.h"
#include "../LZSGData.h"
#include "Based/LZBPlayMusic.h"

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

	controlPlayMusic();
	backgroundLayer();   // 背景层
	informationLayer();  // 信息层
	buttonLayer();       // 按钮层
	controlLayer();      // 控制层
	animationLayer();    // 动画层
	goodsLayer();        // 物品层
	gameTimerLayer();    // 定时层

	pauseGame();

	return true;
}

void HammerZombiesScene::controlPlayMusic()
{
	PlayMusic::changeBgMusic("bgMusic" + to_string(rand() % 11), true);
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

void HammerZombiesScene::pauseGame()
{
	runAction(Sequence::create(DelayTime::create(2.f),
		CallFunc::create([=]() 
			{
				_director->getEventDispatcher()->addCustomEventListener(
					GLViewImpl::EVENT_WINDOW_UNFOCUSED, [&](EventCustom* evt)
					{
						if (!GSPauseQuitLayer::getIsPause())
						{
							PlayMusic::playMusic("pause");
							GSPauseQuitLayer::pauseLayer();
							_director->getOpenGLView()->setCursorVisible(true);
							_director->getRunningScene()->addChild(GSPauseLayer::addLayer(), 10);
						}
					});
			}), nullptr));
}
