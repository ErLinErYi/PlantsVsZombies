/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.01.09
 *Email: 2117610943@qq.com
 */

#include "LZSSJumpLevelLayer.h"
#include "Based/LZBPlayMusic.h"
#include "Based/LZBUserData.h"

#include "Scenes/WorldScene/LZSMModernWorld.h"

SPSJumpLevelLayer::SPSJumpLevelLayer():
	_jumpLevelDialog(nullptr),
	_global(Global::getInstance())
{
}

SPSJumpLevelLayer::~SPSJumpLevelLayer()
{
}

bool SPSJumpLevelLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180))) return false;

	createShieldLayer(this);
	createDialog();
    createText();
	showButton();

	return true;
}

void SPSJumpLevelLayer::createDialog()
{
    _jumpLevelDialog = Sprite::createWithSpriteFrameName("LevelObjiectives.png");
    _jumpLevelDialog->setPosition(Director::getInstance()->getWinSize() / 2.0f);
    _jumpLevelDialog->setGlobalZOrder(10);
    _jumpLevelDialog->setScale(2.0f);
    this->addChild(_jumpLevelDialog);

	showDialogAction(_jumpLevelDialog);
    createTouchtListener(_jumpLevelDialog);
}

void SPSJumpLevelLayer::createText()
{
	auto text = Text::create(
		_global->userInformation->getGameText().find("跳关说明")->second->text, GAME_FONT_NAME_1,
		_global->userInformation->getGameText().find("跳关说明")->second->fontsize);
    text->setColor(Color3B(0, 255, 255));
    text->setPosition(Vec2(_jumpLevelDialog->getContentSize().width / 2, 245));
    text->setGlobalZOrder(10);
    _jumpLevelDialog->addChild(text);

	auto str = _global->userInformation->getGameText().find("跳关说明信息")->second;
	char* buf = new char[str->text.size()];
	snprintf(buf, str->text.size(), str->text.c_str(), UserData::getInstance()->openIntUserData("JUMPLEVELNUMBERS"));

	auto information = Text::create(buf, GAME_FONT_NAME_1, str->fontsize);
    information->setColor(Color3B::RED);
    information->setGlobalZOrder(10);
    information->setTextVerticalAlignment(TextVAlignment::CENTER);
    information->setTextHorizontalAlignment(TextHAlignment::CENTER);
    information->setTextAreaSize(Size(_jumpLevelDialog->getContentSize().width - 85, 150));
    information->setPosition(Vec2(_jumpLevelDialog->getContentSize().width / 2.0f, _jumpLevelDialog->getContentSize().height / 2.0f -30));
    _jumpLevelDialog->addChild(information);

	delete[] buf;
}

void SPSJumpLevelLayer::showButton()
{
	createButton(Vec2(150, 10), _global->userInformation->getGameText().find("确定")->second->text, 1);
	createButton(Vec2(360, 10), _global->userInformation->getGameText().find("取消")->second->text, 2);
}

void SPSJumpLevelLayer::jumpLevel()
{
	caveLevelNumber();

	UserData::getInstance()->caveUserData("JUMPLEVELNUMBERS", UserData::getInstance()->openIntUserData("JUMPLEVELNUMBERS") - 1);

	auto layer = LayerColor::create(Color4B(0, 0, 0, 0));
	layer->setGlobalZOrder(10);
	this->addChild(layer);
	layer->runAction(Sequence::create(FadeIn::create(0.5f),
		CallFunc::create([=]()
			{
				ModernWorld::setPopEnter(true);
				_director->popScene();
			}), nullptr));
}

void SPSJumpLevelLayer::createButton(Vec2& vec2, string title, const int id)
{
	auto button = Button::create("ButtonNew.png", "ButtonNew2.png", "", TextureResType::PLIST);
	button->setPosition(vec2);
	button->setScale(0.25f);
	button->setTitleText(title);
	button->setTitleFontSize(70);
	button->setTitleFontName(GAME_FONT_NAME_1);
	button->setTitleColor(Color3B::BLACK);
	button->setGlobalZOrder(10);
	_jumpLevelDialog->addChild(button);

	UserData::getInstance()->openIntUserData("JUMPLEVELNUMBERS") <= 0 ? id == 1 ? button->setEnabled(false) : nullptr : nullptr;

	button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("gravebutton");
				break;
			case Widget::TouchEventType::ENDED:
				switch (id)
				{
				case 1: jumpLevel();                              break;
				case 2: showDeleteDialogAction(_jumpLevelDialog); break;
				}
			}
		});
}
