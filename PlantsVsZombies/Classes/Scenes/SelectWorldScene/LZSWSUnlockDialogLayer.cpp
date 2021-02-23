/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.17
 *Email: 2117610943@qq.com
 */

#include "LZSWSUnlockDialogLayer.h"
#include "Based/LZPlayMusic.h"

unsigned int SWSUnlockDialogLayer::unlockNeedNumbers = 52;

SWSUnlockDialogLayer::SWSUnlockDialogLayer()
{
	SWSUnlockDialogLayer::unlockNeedNumbers = 52;
}

bool SWSUnlockDialogLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createShieldLayer(this);
	createDiglog();
	createText();
	return true;
}

void SWSUnlockDialogLayer::createDiglog()
{
	_lockDialog = Sprite::createWithSpriteFrameName("LevelObjiectives.png");
	_lockDialog->setPosition(Director::getInstance()->getWinSize() / 2);
	_lockDialog->setScale(2.0f);
	this->addChild(_lockDialog);

	createButtons();

	/* 创建触摸监听 */
	createTouchtListener(_lockDialog);
}

void SWSUnlockDialogLayer::createText()
{
	auto str = _global->userInformation->getGameText().find("大地图模式")->second;
	auto information = Text::create(StringUtils::format(str->text.c_str(), SWSUnlockDialogLayer::unlockNeedNumbers), GAME_FONT_NAME_1, str->fontsize);
	information->setColor(Color3B::RED);
	information->setTextVerticalAlignment(TextVAlignment::CENTER);
	information->setTextHorizontalAlignment(TextHAlignment::CENTER);
	information->setTextAreaSize(Size(_lockDialog->getContentSize().width - 90, 70));
	information->setPosition(Vec2(_lockDialog->getContentSize().width / 2.0f, _lockDialog->getContentSize().height / 2.0f + 10));
	_lockDialog->addChild(information);

	auto str1 = _global->userInformation->getGameText().find("此模式未解锁")->second;
	auto information1 = Text::create(str1->text, GAME_FONT_NAME_1, str1->fontsize);
	information1->setColor(Color3B::RED);
	information1->setPosition(Vec2(_lockDialog->getContentSize().width / 2.0f, _lockDialog->getContentSize().height / 2.0f + 80));
	_lockDialog->addChild(information1);
}

void SWSUnlockDialogLayer::createButtons()
{
	auto button = Button::create("ButtonNew.png", "ButtonNew2.png", "", TextureResType::PLIST);
	auto label = Label::createWithTTF(_global->userInformation->getGameText().find("确定")->second->text,
		GAME_FONT_NAME_1, _global->userInformation->getGameText().find("确定")->second->fontsize);
	label->enableShadow(Color4B(0, 0, 0, 200));//设置阴影
	label->setScale(2.0f);
	button->setTitleLabel(label);
	button->setPosition(Vec2(_lockDialog->getContentSize().width / 2.f, 10));
	button->setScale(0.25f);
	_lockDialog->addChild(button);

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

void SWSUnlockDialogLayer::deleteDialog()
{
	this->removeFromParent();
}