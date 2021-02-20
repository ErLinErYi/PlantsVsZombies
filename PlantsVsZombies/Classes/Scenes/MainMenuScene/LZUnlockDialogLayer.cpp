/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.10
 *Email: 2117610943@qq.com
 */

#include "LZUnlockDialogLayer.h"
#include "Based/LZPlayMusic.h"

unsigned int UnlockDialogLayer::unlockNeedNumbers = 10;

UnlockDialogLayer::UnlockDialogLayer() :
	_lockDialog(nullptr)
{
	unlockNeedNumbers = 10;
}
bool UnlockDialogLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createShieldLayer(this);
	createDiglog();
	createText();
	return true;
}

void UnlockDialogLayer::createDiglog()
{
	_lockDialog = Sprite::createWithSpriteFrameName("QuitDialog.png");
	_lockDialog->setPosition(Director::getInstance()->getWinSize() / 2);
	_lockDialog->setScale(2.0f);
	this->addChild(_lockDialog);

	createButtons();

	/* 创建触摸监听 */
	createTouchtListener(_lockDialog);
}

void UnlockDialogLayer::createButtons()
{
	auto button = Button::create("button.png", "button_down.png", "", TextureResType::PLIST);
	button->setTitleLabel(label(_global->userInformation->getGameText().find("确定")->second->text, 20, Vec2(0, 0), 15, Color3B::GREEN, 1 / 2.f));
	button->setPosition(Vec2(_lockDialog->getContentSize().width / 2.f, 60));
	button->setScaleX(2.f);
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

void UnlockDialogLayer::createText()
{
	auto str = _global->userInformation->getGameText().find("锤僵尸模式")->second;
	auto information = Text::create(StringUtils::format(str->text.c_str(), unlockNeedNumbers), GAME_FONT_NAME_1, str->fontsize);
	information->setColor(Color3B::RED);
	information->setTextVerticalAlignment(TextVAlignment::CENTER);
	information->setTextHorizontalAlignment(TextHAlignment::CENTER);
	information->setTextAreaSize(Size(_lockDialog->getContentSize().width - 90, 70));
	information->setPosition(Vec2(_lockDialog->getContentSize().width / 2.0f, _lockDialog->getContentSize().height / 2.0f + 10));
	_lockDialog->addChild(information);
}

void UnlockDialogLayer::deleteDialog()
{
	setMouseListenerEnable(true);
	this->removeFromParent();
}