/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */

#include "LZHButtonLayer.h"
#include "LZHControlLayer.h"
#include "LZHPauseQuiteLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseQuitLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSDefine.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZUserData.h"

Button* HButtonLayer::stopButton = nullptr;
unsigned int HButtonLayer::selectedHammer = 0;
vector<Button*> HButtonLayer::hammerButton;

HButtonLayer::HammerInformation hammerInformation[7] =
{ 
		{  "D",     2,  100,    500,   0    },
		{  "C",     2,  200,    600,   20   },
		{  "B",     2,  300,    750,   40   },
		{  "A",     1,  400,    900,   55   },
		{  "S",     1,  500,    1100,  60   },
		{  "SS",    0,  600,    1300,  80   },
		{  "SSS",   0,  700,    1500,  100  },
};

HButtonLayer::HButtonLayer() :
	_levelNumber(UserData::getInstance()->openIntUserData("HAMMERZOMBIES_LEVEL_NUMBER"))
{
}

HButtonLayer::~HButtonLayer()
{
}

bool HButtonLayer::init()
{
	if (!Layer::init())return false;

	stopButton = createButton("StopButton", "StopButtonDown", Vec2(1870, 1030), GSLayerButton::stopButton);
	createHammerButton();
	createKeyBoardListener();

	return true;
}

Button* HButtonLayer::createButton(const string& normal, const string& select, const Vec2& position, GSLayerButton buttonName, const bool isFlippedX)
{
	auto button = Button::create(normal + ".png", select + ".png", "", TextureResType::PLIST);
	button->setPosition(position);
	button->setScale(0.7f);
	button->setFlippedX(isFlippedX);
	this->addChild(button);
	button->addTouchEventListener([&, buttonName](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("gravebutton");
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				createQuitDialog();
				_director->getRunningScene()->addChild(HPauseQuitLayer::create(), 10, "pauseLayer");
				break;
			}
		});
	return button;
}

void HButtonLayer::createQuitDialog()
{
	PlayMusic::playMusic("pause");
	GSPauseQuitLayer::pauseLayer();
}

void HButtonLayer::createKeyBoardListener()
{
	/* ´´½¨¼üÅÌ¼àÌý */
	auto keyBoard = EventListenerKeyboard::create();
	keyBoard->onKeyReleased = [&](EventKeyboard::KeyCode code, Event* event)
	{
		switch (code)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:                     /* ¿Õ¸ñ¼üÔÝÍ£ */
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

void HButtonLayer::createHammerButton()
{
	hammerButton.clear();
	selectedHammer = UserData::getInstance()->openIntUserData("SELECTHAMMER");

	for (int i = 0; i < 7; ++i)
	{
		auto button = Button::create("SeedPacket3.png", "SeedPacket3.png", "", TextureResType::PLIST);
		button->setPosition(Vec2(-90, 1030 - 100 * i));
		button->setCascadeColorEnabled(true);
		button->setTag(i);
		this->addChild(button);

		hammerButton.push_back(button);

		loadButtonInforamtion(button);
		button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case cocos2d::ui::Widget::TouchEventType::BEGAN:
					PlayMusic::playMusic("bleep");
					break;
				case cocos2d::ui::Widget::TouchEventType::ENDED:
					selectedHammer = i;
					dynamic_cast<HControlLayer*>(controlLayerInformation)->updateHammerInformation();
					button->getChildByName("SeedPacketFlash")->setColor(Color3B::ORANGE);
					updateHammerButton();
					UserData::getInstance()->caveUserData("SELECTHAMMER", i);
				}
			});
	}
}

void HButtonLayer::loadButtonInforamtion(Button* button)
{
	auto hammer = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("Hammer")->second);
	hammer->setPosition(Vec2(60, 15));
	hammer->setAnimation(0, "Hammer_Normal", false);
	hammer->update(0);
	hammer->setSkin("skin" + to_string(button->getTag()));
	button->addChild(hammer);

	string str[] = { "PlantsLevel_Gold.png","PlantsLevel_Silver.png","PlantsLevel_Copper.png" };
	auto levelIamge = Sprite::createWithSpriteFrameName(str[hammerInformation[button->getTag()].quality]);
	levelIamge->setPosition(Vec2(155, 74));
	levelIamge->setScale(0.65f);
	button->addChild(levelIamge);

	auto prohibit = Sprite::createWithSpriteFrameName("Prohibit.png");
	prohibit->setPosition(Vec2(160, 20));
	prohibit->setScale(0.5f);
	prohibit->setVisible(false);
	button->addChild(prohibit, 1);

	auto attack = ui::Text::create();
	attack->setFontName(GAME_FONT_NAME_3);
	attack->setColor(Color3B::BLACK);
	attack->setAnchorPoint(Vec2(1, 0.5f));
	attack->setPosition(Vec2(175, 18));
	attack->setFontSize(35);
	attack->setString(to_string(hammerInformation[button->getTag()].HigtAttack + hammerInformation[button->getTag()].lowAttack));
	button->addChild(attack);

	auto levelText = ui::Text::create();
	levelText->setFontName(GAME_FONT_NAME_1);
	levelText->setColor(Color3B::BLACK);
	levelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	levelText->setPosition(Vec2(153, 75));
	levelText->setFontSize(25);
	levelText->setString(hammerInformation[button->getTag()].information);
	button->addChild(levelText);

	auto seedPacketFlash = Sprite::createWithSpriteFrameName("SeedPacketFlash.png");
	seedPacketFlash->setPosition(Vec2(0, -1));
	seedPacketFlash->setVisible(false);
	seedPacketFlash->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	seedPacketFlash->setName("SeedPacketFlash");
	button->addChild(seedPacketFlash, 1);

	if (_levelNumber < hammerInformation[button->getTag()].unLock)
	{
		prohibit->setVisible(true);
		button->setEnabled(false);
		button->setColor(Color3B(70, 70, 70));
	}

	if (selectedHammer == button->getTag())
	{
		seedPacketFlash->setVisible(true);
		seedPacketFlash->setColor(Color3B::ORANGE);
	}
}

void HButtonLayer::updateHammerButton()
{
	for (auto button : hammerButton)
	{
		if (button->getTag() != selectedHammer)
		{
			button->getChildByName("SeedPacketFlash")->setColor(Color3B::WHITE);
		}
	}
}
