/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.14
 *Email: 2117610943@qq.com
 */

#include "LZBMSelectPlantsScene.h"
#include "LZBMSPSBackgroundLayer.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSSpriteLayer.h"
#include "Scenes/GameScenes/BigMap/GameScene/LZBigMapGameScene.h"
#include "Based/LZPlayMusic.h"

BMSelectPlantsScene::BMSelectPlantsScene()
{
}

BMSelectPlantsScene::~BMSelectPlantsScene()
{
}

bool BMSelectPlantsScene::init()
{
	if (!Scene::init())return false;

	createBackgroundLayer();
	createControlLayer();
	schedule([this](float) {eventUpdate(_eventType); }, "event");
	schedule([this](float) {_global->checkAnimationInterval(); }, 1.f, "FPS");

	return true;
}

void BMSelectPlantsScene::createBackgroundLayer()
{
	/* 设置容器大小 */
	_scrollLayer = Layer::create();
	_scrollLayer->setContentSize(Size(2930, 1080));

	/* 创建容器中的东西（精灵）*/
	_scrollLayer->addChild(BMSPSBackgroundLayer::create());

	//创建滚动视图
	_scrollView = extension::ScrollView::create(_director->getWinSize(), _scrollLayer);
	_scrollView->setPosition(Vec2(0, 0));
	_scrollView->setDirection(extension::ScrollView::Direction::HORIZONTAL);//设置只能纵向滚动
	_scrollView->setTouchEnabled(false);//关闭触碰事件
	_scrollView->runAction(Sequence::create(DelayTime::create(1.f),
		CallFunc::create([&]()
			{
				_scrollView->setContentOffsetInDuration(Vec2(-1010, 0), 2.5f, 1);
			}), nullptr));
	this->addChild(_scrollView);
}

void BMSelectPlantsScene::controlShowRequirement()
{
	createSelectPlantsDialog();
}

void BMSelectPlantsScene::selectPlantsCallBack()
{
	_scrollView->setContentOffsetInDuration(Vec2(-370, 0), 2.0f, 1);//设置滚动方向与时间

	_eventType = SPSEventType::playGame;
}

void BMSelectPlantsScene::eventUpdate(SPSEventType eventType)
{
	switch (eventType)
	{
	case SPSEventType::scrollToRight:
		if (_scrollView->getContentOffset().x <= SCROLLRIGHTFINISHED)
		{
			_eventType = SPSEventType::null;
			runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([&]() {controlShowRequirement(); }), nullptr));
		}
		break;
	case SPSEventType::scrollToLeft:
		if (_spriteLayer->_selectFinished)
		{
			_eventType = SPSEventType::null;
			selectPlantsCallBack();
		}
		break;
	case SPSEventType::playGame:
		if (_scrollView->getContentOffset().x >= -370)
		{
			/* 播放音乐 */
			_eventType = SPSEventType::null;
			PlayMusic::playMusic("readysetplant");
			createReadyText("StartReady", 1);
		}
		break;
	default: break;
	}
}

void BMSelectPlantsScene::replaceScene()
{
	_global->userInformation->setUserSelectCrads(_spriteLayer->seedBankButton);
	_global->userInformation->setSunNumbers(100); //设定初始阳光数 

	if (_global->userInformation->getIsEaseAnimation() == ui::CheckBox::EventType::SELECTED)
		Director::getInstance()->replaceScene(TransitionFade::create(1.f, BigMapGameScene::create()));
	else
		Director::getInstance()->replaceScene(BigMapGameScene::create());
}