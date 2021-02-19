/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "LZSSControlLayer.h"
#include "LZSSBackgroundLayer.h"
#include "LZSSRequriementLayer.h"
#include "LZSSSpriteLayer.h"
#include "LZSSControlLayer.h"
#include "LZSSSelectPlantsScene.h"
#include "Scenes/GameScene/LZSGGameScene.h"
#include "Scenes/GameScene/LZSGPauseQuitLayer.h"

#include "Based/LZBGameType.h"
#include "Based/LZBPlayMusic.h"

SelectPlantsScene::SelectPlantsScene() :
	_controlLayer(nullptr),
	_scrollLayer(nullptr),
	_spriteLayer(nullptr),
	_scrollView(nullptr),
	_eventType(SPSEventType::scrollToRight),
	_director(Director::getInstance()),
	_global(Global::getInstance())
{
	PlayMusic::changeBgMusic("mainmusic.mo3.1", true);
}

SelectPlantsScene::~SelectPlantsScene()
{
}

bool SelectPlantsScene::init()
{
	if (!Scene::init())return false;

	createBackgroundLayer();
	createControlLayer();
	schedule([this](float) {eventUpdate(_eventType); }, "event");
	schedule([this](float) {_global->checkAnimationInterval(); }, 1.f, "FPS");

	return true;
}

void SelectPlantsScene::createBackgroundLayer()
{
	/* 设置容器大小 */
	_scrollLayer = Layer::create();
	_scrollLayer->setContentSize(Size(2930, 1080));

	/* 创建容器中的东西（精灵）*/
	_scrollLayer->addChild(SPSBackgroundLayer::create());
	
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

void SelectPlantsScene::createControlLayer()
{
	_controlLayer = SPSControlLayer::create();
	this->addChild(_controlLayer, 1);
}

void SelectPlantsScene::eventUpdate(SPSEventType eventType)
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
		if (_scrollView->getContentOffset().x >= SCROLLLEFTFINISHED)
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

void SelectPlantsScene::createSelectPlantsDialog()
{
	/* 删除名字 */
	_controlLayer->removeChildByName("username");

	_spriteLayer = SPSSpriteLayer::create();
	this->addChild(_spriteLayer);

	_eventType = SPSEventType::scrollToLeft;
}

void SelectPlantsScene::controlShowRequirement()
{
	createSelectPlantsDialog();

	/* 如果有要求则显示通过要求 */
	auto _levelData = OpenLevelData::getInstance()->readLevelData(OpenLevelData::getInstance()->getLevelNumber());
	if (_levelData->getGameType().size())
	{
		SPSSpriteLayer::pauseButtonHoverEffect();
		this->addChild(SPSRequriementLayer::create(), 2);
	}
}

void SelectPlantsScene::selectPlantsCallBack()
{
	_scrollView->setContentOffsetInDuration(Vec2(-220, 0), 2.0f, 1);//设置滚动方向与时间

	_eventType = SPSEventType::playGame;
}

void SelectPlantsScene::createReadyText(const std::string& name, const int& id)
{
	static float time[] = { 0,0.2f,0.2f,1.5f };
	auto text = Sprite::createWithSpriteFrameName(name + ".png");
	text->setScale(15.0f);
	text->setName(name);
	text->setPosition(Director::getInstance()->getWinSize() / 2.0f);
	text->runAction(Sequence::create(ScaleTo::create(0.3f, 2.0f), DelayTime::create(time[id]), CallFuncN::create(CC_CALLBACK_1(SelectPlantsScene::readyTextCallBack, this, name, id)), nullptr));
	this->addChild(text);
}

void SelectPlantsScene::readyTextCallBack(Node* node, const std::string& name, const int& id)
{
	this->removeChildByName(name);
	switch (id)
	{
	case 1:
		createReadyText("StartSet", 2);
		break;
	case 2:
		createReadyText("StartPlant", 3);
		break;
	default:
		_scrollLayer->removeChildByName("previewBackgroundImage");

		replaceScene();
		break;
	}
}

void SelectPlantsScene::replaceScene()
{
	_global->userInformation->setUserSelectCrads(_spriteLayer->seedBankButton);
	_global->userInformation->setSunNumbers(100); //设定初始阳光数 

	if (_global->userInformation->getIsEaseAnimation() == ui::CheckBox::EventType::SELECTED)
		Director::getInstance()->replaceScene(TransitionFade::create(1.f, GameScene::create()));
	else
		Director::getInstance()->replaceScene(GameScene::create());
}
