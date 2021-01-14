/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "LZSSControlLayer.h"
#include "LZSSQuitLayer.h"
#include "LZSSSpriteLayer.h"
#include "LZSSJumpLevelLayer.h"
#include "Based/LZBPlayMusic.h"
#include "Based/LZBUserData.h"

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

	showCoinNumbers();
	showDieZombiesNumbers();

	return true;
}

bool SPSControlLayer::isShowJumpLevelButton()
{
	auto gl = Global::getInstance()->userInformation;
	if (getLevelBreakThroughNumbers() >= 6 &&  /* 闯关失败次数多于*次 */
		gl->getUserSelectWorldData().at(gl->getCurrentPlayWorldTag())->levels == gl->getCurrentPlayLevels()){ //目前关卡时最高记录
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
					auto jump = SPSJumpLevelLayer::create();
					if (jump) this->addChild(jump, 11);
					break;
				}
			});
	}
}

void SPSControlLayer::showUserName()
{
	char buff[128];
	snprintf(buff, 128, _global->userInformation->getGameText().find("第 %d 天")->second->text.c_str(), _global->userInformation->getCurrentPlayLevels());

	auto username = Text::create();
	username->setString("“" + _global->userInformation->getUserName() + "”" +
		_global->userInformation->getGameText().find("的时空冒险之旅")->second->text +
		_global->userInformation->getCurrentPlayWorldName() +
		(_global->userInformation->getGameDifficulty() == 1 ? 
			_global->userInformation->getGameText().find("噩梦模式")->second->text + " - " : 
			_global->userInformation->getGameText().find("简单模式")->second->text + " - ") + buff);
	username->setFontName(GAME_FONT_NAME_1);
	username->setFontSize(_global->userInformation->getGameText().find("的时空冒险之旅")->second->fontsize);
	username->setColor(Color3B::YELLOW);
	username->setName("username");
	username->setPosition(Vec2(_director->getWinSize().width / 2.0f, 150));
	this->addChild(username);
}

void SPSControlLayer::showCoinNumbers()
{
	auto coinBank = Sprite::createWithSpriteFrameName("coinbank.png");
	coinBank->setScale(0.8f);
	coinBank->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	coinBank->setPosition(Vec2(1750, 0));
	this->addChild(coinBank);

	auto coinNumbersText = ui::Text::create(to_string(_global->userInformation->getCoinNumbers()), GAME_FONT_NAME_2, 40);
	coinNumbersText->setPosition(Vec2(180, 40));
	coinNumbersText->setAnchorPoint(Vec2(0.5f, 0.5f));
	coinNumbersText->setColor(Color3B::YELLOW);
	coinBank->addChild(coinNumbersText);
}

void SPSControlLayer::showDieZombiesNumbers()
{
	auto zombiesDie = Sprite::createWithSpriteFrameName("ZombiesDie.png");
	zombiesDie->setPosition(Vec2(1500, 0));
	zombiesDie->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	this->addChild(zombiesDie);

	auto zombiesDieText = Text::create(to_string(_global->userInformation->getKillZombiesNumbers()), GAME_FONT_NAME_2, 30);
	zombiesDieText->setPosition(Vec2(130, 32));
	zombiesDieText->setColor(Color3B(190, 0, 190));
	zombiesDie->addChild(zombiesDieText);
}

void SPSControlLayer::createDialog()
{
	SPSSpriteLayer::pauseButtonHoverEffect();
	this->addChild(SPSQuitLayer::create(), 1, "_quitLayer");
}
