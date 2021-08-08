/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.6.28
 *Email: 2117610943@qq.com
 */

#include "LZDonateLayer.h"
#include "Based/LZPlayMusic.h"

DonateLayer::DonateLayer():
	_image(nullptr)
{
}

DonateLayer::~DonateLayer()
{
}

bool DonateLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;
	this->setGlobalZOrder(1);
	createShieldLayer(this);
	createDiglog();
	createText();
	createButtons();

	return true;
}

void DonateLayer::createDiglog()
{
	_image = Sprite::create("resources/Text/Pay.reanim.compiled");
	_image->setPosition(Director::getInstance()->getWinSize() / 2);
	_image->setGlobalZOrder(1);
	this->addChild(_image);
}

void DonateLayer::createText()
{
	auto d = _global->userInformation->getGameText().find("捐赠我们")->second;
	auto donate = Text::create(d->text, GAME_FONT_NAME_1, d->fontsize);
	donate->setColor(Color3B::RED);
	donate->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.f, 900));
	donate->setGlobalZOrder(1);
	this->addChild(donate);

	auto str = _global->userInformation->getGameText().find("捐赠信息说明")->second;
	auto information = Text::create(str->text, GAME_FONT_NAME_1, str->fontsize);
	information->setColor(Color3B::WHITE);
	information->setTextVerticalAlignment(TextVAlignment::CENTER);
	information->setTextHorizontalAlignment(TextHAlignment::CENTER);
	information->setTextAreaSize(Size(1400, 400));
	information->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.f, 800));
	information->setGlobalZOrder(1);
	this->addChild(information);
}

void DonateLayer::createButtons()
{
	auto button = Button::create("ButtonNew.png", "ButtonNew2.png", "", cocos2d::ui::Widget::TextureResType::PLIST);
	auto label = Label::createWithTTF(_global->userInformation->getGameText().find("无情离开")->second->text,
		GAME_FONT_NAME_1, _global->userInformation->getGameText().find("无情离开")->second->fontsize);
	label->enableShadow(Color4B(0, 0, 0, 200));//设置阴影
	label->setScale(2.0f);
	button->setTitleLabel(label);
	button->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.f, 200));
	button->setScale(0.5f);
	button->setGlobalZOrder(1);
	this->addChild(button);

	button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::ENDED:
				PlayMusic::playMusic("gravebutton");
				deleteDialog();
				break;
			}
		});
}

void DonateLayer::deleteDialog()
{
	setMouseListenerEnable(true);
	this->removeFromParent();
}