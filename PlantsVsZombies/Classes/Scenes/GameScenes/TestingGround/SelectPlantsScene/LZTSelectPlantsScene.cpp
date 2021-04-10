/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.07
 *Email: 2117610943@qq.com
 */

#include "LZTSelectPlantsScene.h"
#include "LZTSPSBackgroundLayer.h"
#include "LZTSPSControlLayer.h"
#include "LZTSPSSpriteLayer.h"
#include "LZTSPSRequirementLayer.h"
#include "Scenes/GameScenes/TestingGround/GameScene/LZTestingGroundScene.h"

TSelectPlantsScene::TSelectPlantsScene()
{
}

TSelectPlantsScene::~TSelectPlantsScene()
{
}

bool TSelectPlantsScene::init()
{
	if (!Scene::init())return false;

	createBackgroundLayer();
	createControlLayer();
	schedule([this](float) {eventUpdate(_eventType); }, "event");
	schedule([this](float) {_global->checkAnimationInterval(); }, 1.f, "FPS");

	return true;
}

void TSelectPlantsScene::createBackgroundLayer()
{
	/* 设置容器大小 */
	_scrollLayer = Layer::create();
	_scrollLayer->setContentSize(Size(2930, 1080));

	/* 创建容器中的东西（精灵）*/
	_scrollLayer->addChild(TSPSBackgroundLayer::create());

	//创建滚动视图
	_scrollView = extension::ScrollView::create();
	_scrollView = extension::ScrollView::create(_director->getWinSize(), _scrollLayer);
	_scrollView->setPosition(Vec2(0, 0));
	_scrollView->setDirection(extension::ScrollView::Direction::HORIZONTAL);//设置只能纵向滚动
	_scrollView->setTouchEnabled(false);//关闭触碰事件
	_scrollView->runAction(Sequence::create(DelayTime::create(1.0f),
		CallFunc::create([&]()
			{
				_scrollView->setContentOffsetInDuration(Vec2(-1010, 0), 2.5f, 1);
			}), nullptr));
	this->addChild(_scrollView);
}

void TSelectPlantsScene::controlShowRequirement()
{
	createSelectPlantsDialog();

	SPSSpriteLayer::pauseButtonHoverEffect();
	this->addChild(TSPSRequriementLayer::create(), 2);
}

void TSelectPlantsScene::createSelectPlantsDialog()
{
	/* 删除名字 */
	_controlLayer->removeChildByName("username");

	_spriteLayer = TSPSSpriteLayer::create();
	this->addChild(_spriteLayer);

	_eventType = SPSEventType::scrollToLeft;
}

void TSelectPlantsScene::createControlLayer()
{
	_controlLayer = TSPSControlLayer::create();
	this->addChild(_controlLayer, 1);
}

void TSelectPlantsScene::replaceScene()
{
	_global->userInformation->setUserSelectCrads(_spriteLayer->seedBankButton);
	_global->userInformation->setSunNumbers(0); //设定初始阳光数 

	if (_global->userInformation->getIsEaseAnimation() == ui::CheckBox::EventType::SELECTED)
		Director::getInstance()->replaceScene(TransitionFade::create(1.f, TestingGroundScene::create()));
	else
		Director::getInstance()->replaceScene(TestingGroundScene::create());
}