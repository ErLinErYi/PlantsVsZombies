/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#include "LZTGButtonLayer.h"
#include "LZTGPauseQuitLayer.h"
#include "../SelectPlantsScene/LZTSPSSpriteLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseQuitLayer.h"
#include "Based/LZPlayMusic.h"

TGButtonLayer::TGButtonLayer()
{
}

TGButtonLayer::~TGButtonLayer()
{
}

bool TGButtonLayer::init()
{
	if (!Layer::init())return false;

	showSeedBank();
	showShovelBank();
	createPlantsCard();
	createButton("StopButton", "StopButtonDown", Vec2(1870, 1030), GSLayerButton::stopButton);
	_accelerateButton = createButton("SpeedButton", "SpeedButtonDown", Vec2(1770, 1030), GSLayerButton::accelerateButton);
	_decelerateButton = createButton("SpeedButton", "SpeedButtonDown", Vec2(1670, 1030), GSLayerButton::decelerateButton, true);

	createKeyBoardListener();

	return true;
}

void TGButtonLayer::createQuitDialog()
{
	GSPauseQuitLayer::pauseLayer();
	_director->getRunningScene()->addChild(TGPauseQuitLayer::create(), 10, "pauseLayer");
}

void TGButtonLayer::showShovelBank()
{
	auto shovelBank = Button::create("ShovelBank.png", "", "", TextureResType::PLIST);
	shovelBank->setPosition(Vec2(1520, 1080));
	shovelBank->setScale(0.6f);
	shovelBank->setAnchorPoint(Vec2(0, 1));
	shovelBank->setName("ShovelBank");
	this->addChild(shovelBank);

	/* 铲子监听 */
	shovelBank->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("shovel");
				break;
			case ui::Widget::TouchEventType::ENDED:
				switch (mouseSelectImage->isSelectShovel)
				{
				case true:
					Director::getInstance()->getOpenGLView()->setCursor("resources/images/System/cursor.png", Point::ANCHOR_TOP_LEFT);
					mouseSelectImage->isSelectShovel = false;
					break;
				case false:
					if (_global->userInformation->getIsSelectCursorNotHide() == cocos2d::ui::CheckBox::EventType::UNSELECTED)
						Director::getInstance()->getOpenGLView()->setCursor("resources/images/System/Shovel_hi_res.png", Point::ANCHOR_BOTTOM_LEFT);
					else
						Director::getInstance()->getOpenGLView()->setCursor("resources/images/System/Shovel_hi_res1.png", Vec2(0.125, 0.2f));
					mouseSelectImage->isSelectShovel = true;
					break;
				}
				break;
			}
		});
}

void TGButtonLayer::createKeyBoardListener()
{
	/* 创建键盘监听 */
	auto KeyBoard = EventListenerKeyboard::create();
	KeyBoard->onKeyReleased = [&](EventKeyboard::KeyCode code, Event* event)
	{
		switch (code)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:                     /* 空格键暂停 */
			PlayMusic::playMusic("pause");
			createQuitDialog();
		break;
		case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:                 /* 大小写w或者向上箭头加速 */
		case cocos2d::EventKeyboard::KeyCode::KEY_W:
		case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
			PlayMusic::playMusic("gravebutton");
			controlAccelerateScheduler();
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:                /* 大小写s或者向下箭头减速 */
		case cocos2d::EventKeyboard::KeyCode::KEY_S:
		case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			PlayMusic::playMusic("gravebutton");
			controlDecelerateScheduler();
			break;
		default:
			break;
		}
	};
	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(KeyBoard, this);
}

void TGButtonLayer::createPlantsCard()
{
	int i = -1;
	for (auto& card : _global->userInformation->getUserSelectCrads())
	{
		auto imageBg = "SeedPacket" + to_string(plantsCardInformation[card.cardTag].quality) + ".png";;
		auto cardBackgroundImag = Button::create(imageBg, imageBg, "", TextureResType::PLIST);
		cardBackgroundImag->setPosition(Vec2(105, 1008 - 103 * ++i));
		cardBackgroundImag->setEnabled(false);
		cardBackgroundImag->setColor(Color3B::GRAY);
		cardBackgroundImag->setCascadeColorEnabled(true);
		cardBackgroundImag->setTag(card.cardTag);
		this->addChild(cardBackgroundImag);

		TSPSSpriteLayer* sps_spriteLayer = new TSPSSpriteLayer;
		auto cardInformation = sps_spriteLayer->showPlantsInformation(cardBackgroundImag);
		delete sps_spriteLayer;

		ProgressTimer* timerBar;
		timerBar = createProgressTimer(cardBackgroundImag, 0, 0, card.cardTag);

		SPSSpriteLayer::createButtonHoverEffect(cardBackgroundImag);

		plantsCards[card.cardTag].plantsCards = cardBackgroundImag;
		plantsCards[card.cardTag].tag = card.cardTag;
		plantsCards[card.cardTag].plantsCardText = cardInformation;
		plantsCards[card.cardTag].plantsNeedSunNumbers = 0;
		plantsCards[card.cardTag].progressTimer = timerBar;
	}
}
