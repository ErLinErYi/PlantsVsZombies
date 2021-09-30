/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.10
 *Email: 2117610943@qq.com
 */

#include "LZUnlockDialogLayer.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZUserData.h"

unsigned int UnlockDialogLayer::unlockNeedNumbers = 35;
int UnlockDialogLayer::_id = 0;

UnlockDialogLayer* UnlockDialogLayer::createScene(int id)
{
	_id = id;
	return UnlockDialogLayer::create();
}

UnlockDialogLayer::UnlockDialogLayer() :
	_lockDialog(nullptr)
{
	unlockNeedNumbers = 35;
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
	if (_id != 4)
	{
		auto button = Button::create("button.png", "button_down.png", "", cocos2d::ui::Widget::TextureResType::PLIST);
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
	else
	{
		auto button = Button::create("button.png", "button_down.png", "", cocos2d::ui::Widget::TextureResType::PLIST);
		button->setTitleLabel(label(_global->userInformation->getGameText().find("取消")->second->text, 20, Vec2(0, 0), 15, Color3B::GREEN));
		button->setPosition(Vec2(_lockDialog->getContentSize().width / 2.f - 70, 60));
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

		auto button2 = Button::create("button.png", "button_down.png", "", cocos2d::ui::Widget::TextureResType::PLIST);
		button2->setTitleLabel(label(_global->userInformation->getGameText().find("确定")->second->text, 20, Vec2(0, 0), 15, Color3B::GREEN));
		button2->setPosition(Vec2(_lockDialog->getContentSize().width / 2.f + 70, 60));
		_lockDialog->addChild(button2);

		button2->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
			{
				switch (type)
				{
				case Widget::TouchEventType::ENDED:
					PlayMusic::playMusic("gravebutton");
					if (payCoin)
					{
						if (_global->userInformation->getCoinNumbers() >= 100)
						{
							_global->userInformation->setCoinNumbers(_global->userInformation->getCoinNumbers() - 100);
							UserData::getInstance()->caveUserData(const_cast<char*>("COINNUMBERS"), _global->userInformation->getCoinNumbers());
							payCoin(true);
						}
						else
						{
							((Text*)(_lockDialog->getChildByName("information")))->setString(_global->userInformation->getGameText().find("金币支付失败")->second->text);
							button2->setEnabled(false);
						}
					}
					else
					{
						deleteDialog();
					}
					break;
				}
			});
	}
}

void UnlockDialogLayer::getSelectButton(std::function<void(bool isPay)> pc)
{
	payCoin = pc;
}

void UnlockDialogLayer::createText()
{
	LanguageTextAttribute* str;
	if (_id == 0)
	{
		str = _global->userInformation->getGameText().find("锤僵尸模式")->second;
	}
	else if (_id == 1)
	{
		str = _global->userInformation->getGameText().find("植物试炼场解锁")->second;
	}
	else if (_id == 2)
	{
		str = _global->userInformation->getGameText().find("网络连接失败")->second;
	}
	else if (_id == 3)
	{
		str = _global->userInformation->getGameText().find("我是僵尸解锁")->second;
	}
	else if (_id == 4)
	{
		str = _global->userInformation->getGameText().find("锤僵尸模式支付金币")->second;
	}
	auto information = Text::create(StringUtils::format(str->text.c_str(), unlockNeedNumbers), GAME_FONT_NAME_1, str->fontsize);
	information->setColor(Color3B::RED);
	information->setTextVerticalAlignment(TextVAlignment::CENTER);
	information->setTextHorizontalAlignment(TextHAlignment::CENTER);
	information->setTextAreaSize(Size(_lockDialog->getContentSize().width - 90, 70));
	information->setPosition(Vec2(_lockDialog->getContentSize().width / 2.0f - 10, _lockDialog->getContentSize().height / 2.0f + 10));
	information->setName("information");
	_lockDialog->addChild(information);
}

void UnlockDialogLayer::deleteDialog()
{
	setMouseListenerEnable(true);
	this->removeFromParent();
}