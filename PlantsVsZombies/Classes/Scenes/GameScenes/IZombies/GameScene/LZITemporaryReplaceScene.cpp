/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.21
 *Email: 2117610943@qq.com
 */

#include "LZITemporaryReplaceScene.h"
#include "LZIZombiesScene.h"
#include "Based/LZPlayMusic.h"

IContinueOrExitLayer::IContinueOrExitLayer()
{
}

IContinueOrExitLayer::~IContinueOrExitLayer()
{
}

IContinueOrExitLayer* IContinueOrExitLayer::createScene(int type)
{
	HContinueOrExitLayer::type = type;
	return IContinueOrExitLayer::create();
}

bool IContinueOrExitLayer::init()
{
	if (!Scene::init()) return false;

	//createBackground();
	//createButton("下一关", Vec2(Director::getInstance()->getWinSize().width / 2.f, 100), 2);
	//createText();
	runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([]
		{
			Director::getInstance()->replaceScene(TransitionFade::create(0.3f, IZombiesScene::create()));
		}), nullptr));

	return true;
}

void IContinueOrExitLayer::createButton(const string& text, const Vec2& pos, const int buttonType)
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
					break;
				case 2:
					Director::getInstance()->replaceScene(TransitionFade::create(0.5f, IZombiesScene::create()));
					break;
				default:
					break;
				}
				break;
			}
		});
}

void IContinueOrExitLayer::createText()
{
	auto objiectivesText = Text::create();
	objiectivesText->setString(_global->userInformation->getGameText().find("模式说明")->second->text);
	objiectivesText->setFontName(GAME_FONT_NAME_1);
	objiectivesText->setFontSize(_global->userInformation->getGameText().find("模式说明")->second->fontsize);
	objiectivesText->setColor(Color3B(0, 255, 255));
	objiectivesText->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.f, 980));
	this->addChild(objiectivesText);

	auto str = _global->userInformation->getGameText().find("我是僵尸模式介绍")->second;
	auto information = Text::create(str->text, GAME_FONT_NAME_1, 50);
	information->setColor(Color3B::BLACK);
	information->setGlobalZOrder(10);
	information->setTextVerticalAlignment(TextVAlignment::CENTER);
	information->setTextHorizontalAlignment(TextHAlignment::CENTER);
	information->setTextAreaSize(Size(1100, 800));
	information->setPosition(Director::getInstance()->getWinSize() / 2.0f);
	this->addChild(information);
}
