/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.07
 *Email: 2117610943@qq.com
 */

#include "LZTSPSControlLayer.h"
#include "LZTSPSQuitLayer.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSSpriteLayer.h"
#include "Based/LZPlayMusic.h"

TSPSControlLayer::TSPSControlLayer()
{
}

TSPSControlLayer::~TSPSControlLayer()
{
}

bool TSPSControlLayer::init()
{
	if (!Layer::init()) return false;

	createButton();
	showUserName();

	return true;
}

void TSPSControlLayer::createButton()
{
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
}

void TSPSControlLayer::showUserName()
{
	auto username = Text::create();
	username->setString("“" + _global->userInformation->getUserName() + "”" + _global->userInformation->getGameText().find("植物试炼场")->second->text);
	username->setFontName(GAME_FONT_NAME_1);
	username->setFontSize(_global->userInformation->getGameText().find("噩梦模式")->second->fontsize);
	username->setColor(Color3B::YELLOW);
	username->setName("username");
	username->enableGlow(Color4B::ORANGE);
	username->setPosition(Vec2(_director->getWinSize().width / 2.0f, 150));
	this->addChild(username);
}

void TSPSControlLayer::createDialog()
{
	SPSSpriteLayer::pauseButtonHoverEffect();
	this->addChild(TSPSQuitLayer::create(), 1, "_quitLayer");
}
