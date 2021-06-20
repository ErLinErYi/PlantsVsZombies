/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.14
 *Email: 2117610943@qq.com
 */

#include "LZBigMapGameScene.h"
#include "LZBMGSBackgroundLayer.h"
#include "LZBMGSControlLayer.h"
#include "LZBMGSAnimationLayer.h"
#include "LZBMGSInformationLayer.h"
#include "LZBMGSButtonLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Based/LZMouseEventControl.h"
#include "Based/LZPlayMusic.h"

extension::ScrollView* BigMapGameScene::scrollView = nullptr;

BigMapGameScene::BigMapGameScene()
{
}

BigMapGameScene::~BigMapGameScene()
{
	scrollView = nullptr;
}

bool BigMapGameScene::init()
{
	if (!Scene::init())return false;

	showGameLayer();
	pauseGame();

	isRunGameScene = true;
	return true;
}

void BigMapGameScene::showGameLayer()
{
	controlPlayMusic();
	createScrollView();  // 滑动层
	backgroundLayer();   // 背景层
	informationLayer();  // 信息层
	buttonLayer();       // 按钮层
	controlLayer();      // 控制层
	animationLayer();    // 动画层
	goodsLayer();        // 物品层
	gameTimerLayer();    // 时间层
	setName("GameScene");
}

void BigMapGameScene::createScrollView()
{
	float time = 2.f;
	if (_global->userInformation->getIsReadFileData())time = 0.2f;

	_scrollLayer = Layer::create();
	_scrollLayer->setContentSize(Size(4050, 2160));
	_scrollLayer->setName("scrollLayer");
	_scrollLayer->setScale(0.5f);
	_scrollLayer->setIgnoreAnchorPointForPosition(false);
	_scrollLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_scrollLayer->runAction(Sequence::create(DelayTime::create(time),
		EaseSineOut::create(ScaleTo::create(1.f, 1.f)),
		CallFunc::create([=]()
			{
				_scrollLayer->setIgnoreAnchorPointForPosition(true);
				scrollView->setContentOffset(Vec2(-1120, -540));
				scrollView->setTouchEnabled(true);
			}), nullptr));

	//创建滚动视图
	scrollView = extension::ScrollView::create(_director->getWinSize(), _scrollLayer);
	scrollView->setPosition(Vec2(0, 0));
	scrollView->setBounceable(false);
	scrollView->setTouchEnabled(false);
	scrollView->setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);
	scrollView->setName("scrollView");
	scrollView->setContentOffset(Vec2(0, -540));
	this->addChild(scrollView);
}

void BigMapGameScene::backgroundLayer()
{
	backgroundLayerInformation = BMBackgroundLayer::create();
	backgroundLayerInformation->addLayer(_scrollLayer, 0, "backgroundLayer");
}

void BigMapGameScene::controlLayer()
{
	controlLayerInformation = BMControlLayer::create();
	controlLayerInformation->addLayer(this, 1, "controlLayer");
}

void BigMapGameScene::animationLayer()
{
	animationLayerInformation = BMAnimationLayer::create(_scrollLayer);
	animationLayerInformation->addLayer(_scrollLayer, 2, "animationLayer");
}

void BigMapGameScene::informationLayer()
{
	informationLayerInformation = BMInformationLayer::create();
	informationLayerInformation->addLayer(this, 3, "informationLayer");
}

void BigMapGameScene::goodsLayer()
{
	goodsLayerInformation = Layer::create();
	_scrollLayer->addChild(goodsLayerInformation, 5, "goodsLayer");
	MouseEventControl::goodsRecovery(goodsLayerInformation);
}

void BigMapGameScene::buttonLayer()
{
	buttonLayerInformation = BMButtonLayer::create();
	buttonLayerInformation->addLayer(this, 4, "buttonLayer");
}

void BigMapGameScene::controlPlayMusic()
{
	PlayMusic::changeBgMusic("bgMusic1", true);
}
