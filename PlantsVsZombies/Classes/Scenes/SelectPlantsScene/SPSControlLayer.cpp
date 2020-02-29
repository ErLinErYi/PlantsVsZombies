/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "SPSControlLayer.h"
#include "Scenes/SelectPlantsScene/SPSQuitLayer.h"

SPSControlLayer::SPSControlLayer():
	_quitLayer(nullptr),
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

	return true;
}

void SPSControlLayer::createButton()
{
	/* 创建退出按钮 */
	auto button = ui::Button::create(_global->userInformation->getImagePath().find("StopButton")->second, _global->userInformation->getImagePath().find("StopButtonDown")->second);
	button->setPosition(Vec2(1870, 1030));
	button->setScale(0.7f);
	this->addChild(button);
	button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("gravebutton")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				createDialog();
				break;
			}
		});
}

void SPSControlLayer::showUserName()
{
	char buff[128];
	sprintf(buff, "第 %d 天", _global->userInformation->getCurrentPlayLevels());

	auto username = Text::create();
	username->setString("“" + _global->userInformation->getUserName() + "”" + _global->userInformation->getGameText().find("的时空冒险之旅")->second + _global->userInformation->getCurrentPlayWorldName() + buff);
	username->setFontName("resources/fonts/fzse_gbk.ttf");
	username->setFontSize(60);
	username->setColor(Color3B::YELLOW);
	username->setName("username");
	username->setPosition(Vec2(_director->getWinSize().width / 2.0f, 150));
	this->addChild(username);
}

void SPSControlLayer::createDialog()
{
	_quitLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	_quitLayer->setLocalZOrder(30);
	auto shield = SPSQuitLayer::create();
	shield->addLayer(_quitLayer);
	this->addChild(_quitLayer, 1, "_quitLayer");
}
