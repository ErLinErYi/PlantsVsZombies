/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.15
 *Email: 2117610943@qq.com
 */

#include "LZBMGSButtonLayer.h"
#include "LZBMGSPauseQuitLayer.h"
#include "LZBigMapGameScene.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseQuitLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Plants/DefensePlants/LZSunFlower.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZCoin.h"

BMButtonLayer::BMButtonLayer() :
	_offset(Vec2::ZERO),
	_recoveryCoin(nullptr),
	_recoverySun(nullptr)
{
}

BMButtonLayer::~BMButtonLayer()
{
}

bool BMButtonLayer::init()
{
	if (!Layer::init())return false;

	showSeedBank();
	showShovelBank();
	showNextWaveButton();
	createPlantsCard();
	createRequirementButton();
	createButton("StopButton", "StopButtonDown", Vec2(1870, 1030), GSLayerButton::stopButton);
	_recoverySun = createButton("RecoverySun2", "RecoverySun", Vec2(550, 1025), GSLayerButton::recoverySunButton);
	_recoveryCoin = createButton("RecoveryCoin2", "RecoveryCoin", Vec2(650, 1025), GSLayerButton::recoveryCoinButton);
	_accelerateButton = createButton("SpeedButton", "SpeedButtonDown", Vec2(1770, 1030), GSLayerButton::accelerateButton);
	_decelerateButton = createButton("SpeedButton", "SpeedButtonDown", Vec2(1670, 1030), GSLayerButton::decelerateButton, true);

	schedule([&](float) { checkSunAndCoin(); }, 1.0f, "checkSunAndCoin");
	createKeyBoardListener();

	return true;
}

void BMButtonLayer::createQuitDialog()
{
	GSPauseQuitLayer::pauseLayer();
	_director->getRunningScene()->addChild(BMPauseQuitLayer::create(), 10, "pauseLayer");
}

void BMButtonLayer::createKeyBoardListener()
{
	auto keyBoard = EventListenerKeyboard::create();
	//keyBoard->setEnabled(false);

	//keyBoard->onKeyPressed = [&](EventKeyboard::KeyCode code, Event* event)
	//{
	//	switch (code)
	//	{
	//	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:                 /* 大小写w或者向上箭头加速 */
	//	case cocos2d::EventKeyboard::KeyCode::KEY_W:
	//	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
	//		this->runAction(RepeatForever::create(Sequence::create(
	//			CallFunc::create([&]()
	//				{
	//					_offset = BigMapGameScene::scrollView->getContentOffset();
	//					_offset = Vec2(_offset.x, _offset.y - 20);
	//					changeScrollViewOffset();
	//				}), DelayTime::create(0.02f), nullptr)));
	//		break;
	//	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:                /* 大小写s或者向下箭头减速 */
	//	case cocos2d::EventKeyboard::KeyCode::KEY_S:
	//	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	//		this->runAction(RepeatForever::create(Sequence::create(
	//			CallFunc::create([&]()
	//				{
	//					_offset = BigMapGameScene::scrollView->getContentOffset();
	//					_offset = Vec2(_offset.x, _offset.y + 20);
	//					changeScrollViewOffset();
	//				}), DelayTime::create(0.02f), nullptr)));
	//		break;
	//	case cocos2d::EventKeyboard::KeyCode::KEY_A:
	//	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_A:
	//	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	//		this->runAction(RepeatForever::create(Sequence::create(
	//			CallFunc::create([&]()
	//				{
	//					_offset = BigMapGameScene::scrollView->getContentOffset();
	//					_offset = Vec2(_offset.x + 20, _offset.y);
	//					changeScrollViewOffset();
	//				}), DelayTime::create(0.02f), nullptr)));
	//		break;
	//	case cocos2d::EventKeyboard::KeyCode::KEY_D:
	//	case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_D:
	//	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	//		this->runAction(RepeatForever::create(Sequence::create(
	//			CallFunc::create([&]()
	//				{
	//					_offset = BigMapGameScene::scrollView->getContentOffset();
	//					_offset = Vec2(_offset.x - 20, _offset.y);
	//					changeScrollViewOffset();
	//				}), DelayTime::create(0.02f), nullptr)));
	//		break;
	//	default:
	//		break;
	//	}
	//};

	keyBoard->onKeyReleased = [&](EventKeyboard::KeyCode code, Event* event)
	{
		switch (code)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:                     /* 空格键暂停 */
			GSPauseQuitLayer::pauseLayer();
			PlayMusic::playMusic("pause");
			_director->getRunningScene()->addChild(GSPauseLayer::addLayer(), 10);
			break;
		default:
			break;
		}
		this->stopAllActions();
	};
	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyBoard, this);

	//runAction(Sequence::create(DelayTime::create(3.1f),
	//	CallFunc::create([=]()
	//		{
	//			keyBoard->setEnabled(true);
	//		}), nullptr));
}

void BMButtonLayer::createRequirementButton()
{
	if (SPSControlLayer::isShowJumpLevelButton())
	{
		createJumpLevelButton();
	}
}

void BMButtonLayer::showShovelBank()
{
	GSButtonLayer::showShovelBank();
	this->getChildByName("ShovelBank")->setPosition(Vec2(1525, 1080));
}

void BMButtonLayer::recoverySunControl()
{
	for (auto& sun : SunsGroup)
	{
		if (sun->getEnable() && sun->getSun()->getOpacity() > 200)
		{
			PlayMusic::playMusic("points");
			SunFlower::sunRecovery(sun);
			break;
		}
	}
}

void BMButtonLayer::recoveryCoinControl()
{
	for (auto& coin : CoinsGroup)
	{
		if (coin->getEnable() && coin->getCoin()->getOpacity() > 200)
		{
			PlayMusic::playMusic("coin");
			Coin::coinRecoveryAction(coin);
			break;
		}
	}
}

void BMButtonLayer::changeScrollViewOffset()
{
	BigMapGameScene::scrollView->setContentOffset(_offset);
}

void BMButtonLayer::checkSunAndCoin()
{
	for (auto& sun : SunsGroup)
	{
		if (sun->getEnable() && sun->getSun()->getOpacity() > 200)
		{
			_recoverySun->runAction(Sequence::create(TintTo::create(0.5f, Color3B(255, 100, 100)), 
				TintTo::create(0.5f, Color3B::WHITE), nullptr));
			break;
		}
	}
	for (auto& coin : CoinsGroup)
	{
		if (coin->getEnable() && coin->getCoin()->getOpacity() > 200)
		{
			_recoveryCoin->runAction(Sequence::create(TintTo::create(0.5f, Color3B(255, 100, 100)), 
				TintTo::create(0.5f, Color3B::WHITE), nullptr));
			break;
		}
	}
}
