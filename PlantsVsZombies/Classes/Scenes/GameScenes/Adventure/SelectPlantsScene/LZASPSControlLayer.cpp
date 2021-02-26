/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "LZASPSControlLayer.h"
#include "LZASPSQuitLayer.h"
#include "LZASPSSpriteLayer.h"
#include "LZASPSJumpLevelLayer.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZUserData.h"

SPSControlLayer::SPSControlLayer():
	_global(Global::getInstance())
{
}

SPSControlLayer::~SPSControlLayer()
{
}

bool SPSControlLayer::init()
{
	if (!Layer::init()) return false;

	/* 创建按钮 */
	createButton();

	/* 显示用户名字 */
	showUserName();

	showCoinNumbers(this);
	showDieZombiesNumbers(this);

	return true;
}

bool SPSControlLayer::isShowJumpLevelButton()
{
	auto gl = Global::getInstance()->userInformation;
	if (getLevelBreakThroughNumbers() >= 6 &&  /* 闯关失败次数多于*次 */
		gl->getUserSelectWorldData().at(
		gl->getCurrentPlayWorldTag())->levels == 
		gl->getCurrentPlayLevels())            /* 目前关卡时最高记录 */
	{ 
		return true;
	}
	return false;
}

int SPSControlLayer::getLevelBreakThroughNumbers()
{
	return UserData::getInstance()->openIntUserData(Global::getInstance()->userInformation->getCurrentCaveFileLevelWorldName());
}

void SPSControlLayer::createButton()
{
	/* 创建退出按钮 */
	auto button = ui::Button::create("StopButton.png", "StopButtonDown.png", "", TextureResType::PLIST);
	button->setPosition(Vec2(1870, 1030));
	button->setScale(0.7f);
	this->addChild(button);
	button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("gravebutton");
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				createDialog();
				break;
			}
		});

	if (isShowJumpLevelButton())
	{
		auto breakButton = ui::Button::create("SeedChooser_Button.png", "SeedChooser_Button_Disabled.png", "", TextureResType::PLIST);
		breakButton->setPosition(Vec2(1250, 5));
		breakButton->setScale(1.5f);
		breakButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		breakButton->setTitleText(_global->userInformation->getGameText().find("跳过此关")->second->text);
		breakButton->setTitleColor(Color3B(0, 255, 255));
		breakButton->setTitleFontName(GAME_FONT_NAME_1);
		breakButton->setTitleFontSize(_global->userInformation->getGameText().find("跳过此关")->second->fontsize);
		this->addChild(breakButton);
		breakButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case cocos2d::ui::Widget::TouchEventType::BEGAN:
					PlayMusic::playMusic("gravebutton");
					break;
				case cocos2d::ui::Widget::TouchEventType::ENDED:
					SPSSpriteLayer::pauseButtonHoverEffect();
					auto jump = SPSJumpLevelLayer::create();
					if (jump) this->addChild(jump, 11);
					break;
				}
			});

		srand(time(nullptr));
		auto effect = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("ButtonEffect")->second);
		effect->setAnimation(0, rand() % 2 ? "blue" : "yellow", true);
		effect->setPosition(breakButton->getContentSize() / 2.f);
		effect->setScale(2.05f,0.49f);
		effect->setTimeScale(0.8f);
		effect->update(0);
		breakButton->addChild(effect);
	}
}

void SPSControlLayer::showUserName()
{
	auto username = Text::create();
	username->setString("“" + _global->userInformation->getUserName() + "”" +
		_global->userInformation->getGameText().find("的时空冒险之旅")->second->text +
		_global->userInformation->getCurrentPlayWorldName() +
		(_global->userInformation->getGameDifficulty() == 1 ? 
			_global->userInformation->getGameText().find("噩梦模式")->second->text + " - " : 
			_global->userInformation->getGameText().find("简单模式")->second->text + " - ") + 
		StringUtils::format(_global->userInformation->getGameText().find("第 %d 天")->second->text.c_str(), 
			_global->userInformation->getCurrentPlayLevels()));
	username->setFontName(GAME_FONT_NAME_1);
	username->setFontSize(_global->userInformation->getGameText().find("噩梦模式")->second->fontsize);
	username->setColor(Color3B::YELLOW);
	username->setName("username");
	username->enableGlow(Color4B::ORANGE);
	username->setPosition(Vec2(_director->getWinSize().width / 2.0f, 150));
	this->addChild(username);
}

ui::Text* SPSControlLayer::showCoinNumbers(Node* node)
{
	auto coinBank = Sprite::createWithSpriteFrameName("coinbank.png");
	coinBank->setScale(0.8f);
	coinBank->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	coinBank->setPosition(Vec2(1750, 0));
	node->addChild(coinBank);

	auto coinNumbersText = ui::Text::create(to_string(Global::getInstance()->userInformation->getCoinNumbers()), GAME_FONT_NAME_2, 35);
	coinNumbersText->setPosition(Vec2(195, 36));
	coinNumbersText->setTextAreaSize(Size(230, coinBank->getContentSize().height - 40));
	coinNumbersText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	coinNumbersText->setTextVerticalAlignment(TextVAlignment::CENTER);
	coinNumbersText->setAnchorPoint(Vec2(0.5f, 0.5f));
	coinNumbersText->setColor(Color3B::YELLOW);
	coinNumbersText->enableGlow(Color4B(0, 255, 255, 255));
	coinNumbersText->setOverflow(Overflow::SHRINK);
	coinBank->addChild(coinNumbersText);

	return coinNumbersText;
}

ui::Text* SPSControlLayer::showDieZombiesNumbers(Node* node)
{
	auto zombiesDie = Sprite::createWithSpriteFrameName("ZombiesDie.png");
	zombiesDie->setPosition(Vec2(1500, 0));
	zombiesDie->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	node->addChild(zombiesDie);

	auto zombiesDieText = Text::create(to_string(Global::getInstance()->userInformation->getKillZombiesNumbers()), GAME_FONT_NAME_2, 30);
	zombiesDieText->setPosition(Vec2(135, 30));
	zombiesDieText->setTextAreaSize(Size(140, zombiesDie->getContentSize().height - 40));
	zombiesDieText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	zombiesDieText->setTextVerticalAlignment(TextVAlignment::CENTER);
	zombiesDieText->setColor(Color3B(190, 0, 190));
	zombiesDieText->enableGlow(Color4B(0, 255, 255, 255));
	zombiesDieText->setOverflow(Overflow::SHRINK);
	zombiesDie->addChild(zombiesDieText);

	return zombiesDieText;
}

void SPSControlLayer::createDialog()
{
	SPSSpriteLayer::pauseButtonHoverEffect();
	this->addChild(SPSQuitLayer::create(), 1, "_quitLayer");
}
