/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.09.21
 *Email: 2117610943@qq.com
 */

#include "LZHContinueOrExitLayer.h"
#include "LZHammerZombiesScene.h"
#include "Scenes/SelectWorldScene/LZSelectWorldScene.h"
#include "Scenes/MainMenuScene/LZMainMenu.h"
#include "Based/LZPlayMusic.h"

int HContinueOrExitLayer::type = 0;

HContinueOrExitLayer::HContinueOrExitLayer():
	_global(Global::getInstance())
{
}

HContinueOrExitLayer* HContinueOrExitLayer::createScene(int type)
{
	HContinueOrExitLayer::type = type;
	return HContinueOrExitLayer::create();
}

HContinueOrExitLayer::~HContinueOrExitLayer()
{
	HContinueOrExitLayer::type = 0;
}

bool HContinueOrExitLayer::init()
{
	if (!Scene::init()) return false;

	createBackground();
	createText();

	auto size = Director::getInstance()->getWinSize();
	
	if (HContinueOrExitLayer::type == 1)
	{
		createButton("主菜单", Vec2(size.width / 2.f - 200, 100), 1);
		createButton("下一关", Vec2(size.width / 2.f + 200, 100), 2);
	}
	else
	{
		createButton("主菜单", Vec2(size.width / 2.f - 200, 100), 1);
		createButton("重新开始", Vec2(size.width / 2.f + 200, 100), 2);
	}

	return true;
}

void HContinueOrExitLayer::createBackground()
{
	SelectWorldScene::createGalaxy(this);
}

void HContinueOrExitLayer::createButton(const string& text, const Vec2& pos, const int buttonType)
{
	/* 创建按钮 */
	auto button = Button::create("ButtonNew2.png", "ButtonNew.png", "", cocos2d::ui::Widget::TextureResType::PLIST);
	button->setTitleText(_global->userInformation->getGameText().find(text)->second->text);
	button->setTitleColor(Color3B(0, 255, 255));
	button->setTitleFontSize(50);
	button->setScale(.7f);
	button->setPosition(pos);
	this->addChild(button);
	button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap2");
				break;
			case Widget::TouchEventType::ENDED:
				switch (buttonType)
				{
				case 1:
					Director::getInstance()->replaceScene(MainMenu::create());
					break;
				case 2:
					Director::getInstance()->replaceScene(TransitionFade::create(0.5f, HammerZombiesScene::create()));
					break;
				default:
					break;
				}
				break;
			}
		});
}

void HContinueOrExitLayer::createText()
{
	auto objiectivesText = Text::create();
	objiectivesText->setString(_global->userInformation->getGameText().find("模式说明")->second->text);
	objiectivesText->setFontName(GAME_FONT_NAME_1);
	objiectivesText->setFontSize(_global->userInformation->getGameText().find("模式说明")->second->fontsize);
	objiectivesText->setColor(Color3B(0, 255, 255));
	objiectivesText->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.f, 980));
	this->addChild(objiectivesText);

	auto str = _global->userInformation->getGameText().find("锤僵尸模式介绍")->second;
	auto information = Text::create(str->text, GAME_FONT_NAME_1, str->fontsize);
	information->setColor(Color3B::BLACK);
	information->setGlobalZOrder(10);
	information->setTextVerticalAlignment(TextVAlignment::CENTER);
	information->setTextHorizontalAlignment(TextHAlignment::CENTER);
	information->setTextAreaSize(Size(1100, 800));
	information->setPosition(Director::getInstance()->getWinSize() / 2.0f);
	this->addChild(information);
}