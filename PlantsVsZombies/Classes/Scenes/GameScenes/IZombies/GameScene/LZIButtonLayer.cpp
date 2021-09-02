/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.02
 *Email: 2117610943@qq.com
 */

#include "LZIButtonLayer.h"
#include "LZIPauseQuiteLayer.h"
#include "LZIRequirementLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseLayer.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSSpriteLayer.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZUserData.h"

Button* IButtonLayer::stopButton = nullptr;
vector<IButtonLayer::ZombiesCardButton> IButtonLayer::zombiesCardButton;
IButtonLayer::ZombiesCardButton zombiesCardInformation[9] =
{
		{  nullptr,  "LmpZombie",             "D",     2,   50,  0, ZombiesType::LmpZombies               },
		{  nullptr,  "ConeZombie",            "C",     2,   100, 1, ZombiesType::ConeZombies              },
		{  nullptr,  "NewPaperZombie",        "C",     2,   150, 2, ZombiesType::StrongNewspaperZombies   },
		{  nullptr,  "DoorZombie",            "B",     2,   200, 3, ZombiesType::CommonDoorZombies        },
		{  nullptr,  "BucketZombie",          "B",     2,   200, 4, ZombiesType::BucketZombies            },
		{  nullptr,  "BrickZombie",           "A",     1,   275, 5, ZombiesType::BrickZombies             },
		{  nullptr,  "BucketDoorZombie",      "S",     1,   300, 6, ZombiesType::Bucket4Door4Zombies      },
		{  nullptr,  "BalloonZombie",         "SS",    0,   350, 7, ZombiesType::BalloonZombies           },
		{  nullptr,  "GargantuarZombie",      "SSS",   0,   500, 8, ZombiesType::GargantuarZombies        },
};

IButtonLayer::IButtonLayer() :
	_levelNumber(UserData::getInstance()->openIntUserData(const_cast<char*>("HAMMERZOMBIES_LEVEL_NUMBER")))
{
}

IButtonLayer::~IButtonLayer()
{
}

bool IButtonLayer::init()
{
	if (!Layer::init())return false;

	showSeedBank();
	createZombiesCard();
	stopButton = createButton("StopButton", "StopButtonDown", Vec2(1870, 1030), GSLayerButton::stopButton);
	_accelerateButton = createButton("SpeedButton", "SpeedButtonDown", Vec2(1770, 1030), GSLayerButton::accelerateButton);
	createButton("Requirement", "RequirementDown", Vec2(1670, 1030), GSLayerButton::informationButton);
	createKeyBoardListener();

	return true;
}

Button* IButtonLayer::createButton(const string& normal, const string& select, const Vec2& position, GSLayerButton buttonName, const bool isFlippedX)
{
	auto button = Button::create(normal + ".png", select + ".png", "", cocos2d::ui::Widget::TextureResType::PLIST);
	button->setPosition(position);
	button->setScale(0.7f);
	button->setFlippedX(isFlippedX);
	this->addChild(button);
	button->addTouchEventListener([&, buttonName](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				switch (buttonName)
				{
				case GSLayerButton::stopButton:
					createQuitDialog();
					_director->getRunningScene()->addChild(IPauseQuitLayer::create(), 10, "pauseQuitLayer");
					break;
				case GSLayerButton::accelerateButton:
					controlAccelerateScheduler();
					break;
				case GSLayerButton::informationButton:
					showModeInformation();
					break;
				}
				break;
			}
		});
	return button;
}

void IButtonLayer::createQuitDialog()
{
	GSPauseQuitLayer::pauseLayer();
	PlayMusic::playMusic("pause");
}

void IButtonLayer::createKeyBoardListener()
{
	/* 创建键盘监听 */
	auto keyBoard = EventListenerKeyboard::create();
	keyBoard->onKeyReleased = [&](EventKeyboard::KeyCode code, Event* event)
	{
		switch (code)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:                     /* 空格键暂停 */
			createQuitDialog();
			_director->getOpenGLView()->setCursorVisible(true);
			_director->getRunningScene()->addChild(GSPauseLayer::create(), 10, "pauseLayer");
			break;
		default:
			break;
		}
	};
	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyBoard, this);
}

void IButtonLayer::createZombiesCard()
{
	zombiesCardButton.clear();

	for (unsigned int i = 0; i < 9; ++i)
	{
		auto imageBg = "SeedPacket" + to_string(zombiesCardInformation[i].quality + 1) + ".png";
		auto cardBackgroundImag = Button::create(imageBg, imageBg, "", cocos2d::ui::Widget::TextureResType::PLIST);
		cardBackgroundImag->setPosition(Vec2(105, 1008 - 103 * i));
		//cardBackgroundImag->setEnabled(false);
		cardBackgroundImag->setColor(Color3B::GRAY);
		cardBackgroundImag->setCascadeColorEnabled(true);
		this->addChild(cardBackgroundImag);

		showZombiesInformation(cardBackgroundImag, i);

		SPSSpriteLayer::createButtonHoverEffect(cardBackgroundImag);

		ZombiesCardButton zcb;
		zcb = zombiesCardInformation[i];
		zcb.button = cardBackgroundImag;
		zombiesCardButton.push_back(zcb);
	}
}

void IButtonLayer::showZombiesInformation(Button* button, int type)
{
	/* 图片 */
	auto plantImage = Sprite::createWithSpriteFrameName(zombiesCardInformation[type].information[0] + ".png");
	plantImage->setScale(0.8f);
	plantImage->setPosition(Vec2(85, 50));
	button->addChild(plantImage);

	string str[] = { "PlantsLevel_Gold.png","PlantsLevel_Silver.png","PlantsLevel_Copper.png" };
	auto plantsLevelIamge = Sprite::createWithSpriteFrameName(str[zombiesCardInformation[type].quality]);
	plantsLevelIamge->setPosition(Vec2(155, 74));
	plantsLevelIamge->setScale(0.65f);
	button->addChild(plantsLevelIamge);

	/*
	auto prohibit = Sprite::createWithSpriteFrameName("Prohibit.png");
	prohibit->setPosition(Vec2(160, 20));
	prohibit->setScale(0.5f);
	prohibit->setVisible(!isPlantIsCanSelect[static_cast<unsigned int>(plantsCardInformation[button->getTag()].type)]);
	button->addChild(prohibit, 1);*/

	/* 文本 */
	auto plantsNeedSuns = cocos2d::ui::Text::create();
	plantsNeedSuns->setFontName(GAME_FONT_NAME_3);
	plantsNeedSuns->setColor(Color3B::WHITE);
	plantsNeedSuns->setAnchorPoint(Vec2(1, 0.5f));
	plantsNeedSuns->setPosition(Vec2(175, 18));
	plantsNeedSuns->setFontSize(30);
	plantsNeedSuns->setString(to_string(zombiesCardInformation[type].needSunNumber));
	plantsNeedSuns->enableOutline(Color4B::BLACK, 2);
	button->addChild(plantsNeedSuns);

	auto plantsLevelText = cocos2d::ui::Text::create();
	plantsLevelText->setFontName(GAME_FONT_NAME_1);
	plantsLevelText->setColor(Color3B::BLACK);
	plantsLevelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	plantsLevelText->setPosition(Vec2(153, 75));
	plantsLevelText->setFontSize(25);
	plantsLevelText->setString(zombiesCardInformation[type].information[1]);
	button->addChild(plantsLevelText);
}

void IButtonLayer::showModeInformation()
{
	Director::getInstance()->getRunningScene()->addChild(IRequriementLayer::create(),10);
}
