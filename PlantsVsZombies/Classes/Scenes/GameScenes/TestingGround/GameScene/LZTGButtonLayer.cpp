/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#include "LZTGButtonLayer.h"
#include "LZTGPauseQuitLayer.h"
#include "LZTRankingListLayer.h"
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

	auto rankList = createButton("ButtonNew", "ButtonNew2", Vec2(1365, 1030), GSLayerButton::rankingListButton);
	rankList->setTitleText(GAME_TEXT("闯关记录排行榜"));
	rankList->setTitleFontSize(GAME_TEXT_SIZE("闯关记录排行榜"));
	rankList->setTitleFontName(GAME_FONT_NAME_1);
	rankList->setTitleColor(Color3B::BLACK);

	createKeyBoardListener();

	return true;
}

void TGButtonLayer::createQuitDialog()
{
	GSPauseQuitLayer::pauseLayer();
	PlayMusic::playMusic("pause");
	_director->getRunningScene()->addChild(TGPauseQuitLayer::create(), 10, "pauseLayer");
}

void TGButtonLayer::showShovelBank()
{
	_shovelBank = Button::create("ShovelBank.png", "", "",cocos2d::ui::Widget::TextureResType::PLIST);
	_shovelBank->setPosition(Vec2(1520, 1075));
	_shovelBank->setScale(0.6f);
	_shovelBank->setAnchorPoint(Vec2(0, 1));
	_shovelBank->setName("ShovelBank");
	this->addChild(_shovelBank);

	/* 铲子监听 */
	_shovelBank->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("shovel");
				break;
			case ui::Widget::TouchEventType::ENDED:
				switch (mouseSelectImage->isSelectShovel)
				{
				case true:  deleteShovelImage();break;
				case false: createShovelImage();break;
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
	int t = 0;
	for (int i = _global->userInformation->getUserSelectCrads().size() - 1; i >= 0; --i)
	{
		if (_global->userInformation->getUserSelectCrads()[i].cardTag == static_cast<int>(PlantsType::Imitater))
		{
			t = i - 1;
			break;
		}
	}
	auto n = _global->userInformation->getUserSelectCrads()[t].cardTag;
	auto m = _global->userInformation->getUserSelectCrads()[t + 1].cardTag;

	int i = -1;
	for (auto& card : _global->userInformation->getUserSelectCrads())
	{
		auto imageBg = "SeedPacket" + to_string(plantsCardInformation[card.cardTag].quality) + ".png";;
		auto cardBackgroundImag = Button::create(imageBg, imageBg, "",cocos2d::ui::Widget::TextureResType::PLIST);
		cardBackgroundImag->setPosition(Vec2(105, 1008 - 103 * ++i));
		cardBackgroundImag->setEnabled(false);
		cardBackgroundImag->setColor(Color3B::GRAY);
		cardBackgroundImag->setCascadeColorEnabled(true);
		cardBackgroundImag->setTag(card.cardTag);
		this->addChild(cardBackgroundImag);

		TSPSSpriteLayer* sps_spriteLayer = new TSPSSpriteLayer;
		Text* cardInformation = nullptr;
		if (plantsCardInformation[card.cardTag].type == PlantsType::Imitater)
		{
			cardBackgroundImag->setTag(_global->userInformation->getUserSelectCrads()[t].cardTag);
			cardInformation = sps_spriteLayer->showPlantsInformation(cardBackgroundImag);
			cardBackgroundImag->getChildByName("plantImage")->setGLProgram(GSButtonLayer::greyScaleShader());
			cardBackgroundImag->setTag(card.cardTag);
		}
		else
		{
			cardInformation = sps_spriteLayer->showPlantsInformation(cardBackgroundImag);
		}
		delete sps_spriteLayer;

		ProgressTimer* timerBar;
		timerBar = createProgressTimer(cardBackgroundImag, 0, 0, card.cardTag);

		SPSSpriteLayer::createButtonHoverEffect(cardBackgroundImag);

		plantsCards[card.cardTag].plantsCards = cardBackgroundImag;
		plantsCards[card.cardTag].tag = card.cardTag;
		plantsCards[card.cardTag].plantsCardText = cardInformation;
		plantsCards[card.cardTag].plantsNeedSunNumbers = 0;
		plantsCards[card.cardTag].plantsCoolTime = 0;
		plantsCards[card.cardTag].progressTimer = timerBar;
	}
}

void TGButtonLayer::onShowRankingLayer()
{
	GSPauseQuitLayer::pauseLayer();
	PlayMusic::playMusic("tap2");
	_director->getRunningScene()->addChild(TRankingListLayer::create(), 5, "RankingListLayer");
}
