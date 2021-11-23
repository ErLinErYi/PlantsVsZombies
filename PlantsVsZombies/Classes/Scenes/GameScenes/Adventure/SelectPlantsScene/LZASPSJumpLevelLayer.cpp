/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.01.09
 *Email: 2117610943@qq.com
 */

#include "LZASPSJumpLevelLayer.h"
#include "LZASPSSpriteLayer.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZUserData.h"
#include "Based/LZDefine.h"

#include "Scenes/GameScenes/Adventure/WorldScene/LZModernWorld.h"

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
	auto text = Text::create(GAME_TEXT("跳关说明"), GAME_FONT_NAME_1, GAME_TEXT_SIZE("跳关说明"));
    text->setColor(Color3B(0, 255, 255));
    text->setPosition(Vec2(_jumpLevelDialog->getContentSize().width / 2, 245));
    text->setGlobalZOrder(10);
    _jumpLevelDialog->addChild(text);

	auto information = Text::create(StringUtils::format(GAME_TEXT("跳关说明信息").c_str(),
		UserData::getInstance()->openIntUserData(const_cast<char*>("JUMPLEVELNUMBERS"))), GAME_FONT_NAME_1, GAME_TEXT_SIZE("跳关说明信息"));
    information->setColor(Color3B::RED);
    information->setGlobalZOrder(10);
    information->setTextVerticalAlignment(TextVAlignment::CENTER);
    information->setTextHorizontalAlignment(TextHAlignment::CENTER);
    information->setTextAreaSize(Size(_jumpLevelDialog->getContentSize().width - 85, 150));
    information->setPosition(Vec2(_jumpLevelDialog->getContentSize().width / 2.0f, _jumpLevelDialog->getContentSize().height / 2.0f -30));
    _jumpLevelDialog->addChild(information);
}

void SPSJumpLevelLayer::showButton()
{
	createButton(Vec2(150, 10), GAME_TEXT("确定"), 1);
	createButton(Vec2(360, 10), GAME_TEXT("取消"), 2);
}

void SPSJumpLevelLayer::jumpLevel()
{
	caveLevelNumber();

	UserData::getInstance()->caveUserData(const_cast<char*>("JUMPLEVELNUMBERS"), UserData::getInstance()->openIntUserData(const_cast<char*>("JUMPLEVELNUMBERS")) - 1);

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

void SPSJumpLevelLayer::createButton(const Vec2& vec2, string title, const int id)
{
	auto button = Button::create("ButtonNew.png", "ButtonNew2.png", "",cocos2d::ui::Widget::TextureResType::PLIST);
	button->setPosition(vec2);
	button->setScale(0.25f);
	button->setTitleText(title);
	button->setTitleFontSize(70);
	button->setTitleFontName(GAME_FONT_NAME_1);
	button->setTitleColor(Color3B::BLACK);
	button->setGlobalZOrder(10);
	_jumpLevelDialog->addChild(button);

	if (UserData::getInstance()->openIntUserData(const_cast<char*>("JUMPLEVELNUMBERS")) <= 0 && id == 1)
	{
		button->setEnabled(false);
	}

	button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("gravebutton");
				break;
			case Widget::TouchEventType::ENDED:
				SPSSpriteLayer::resumeButtonHoverEffect();
				switch (id)
				{
				case 1: jumpLevel();                              break;
				case 2: showDeleteDialogAction(_jumpLevelDialog); break;
				}
			}
		});
}
