/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.09
 *Email: 2117610943@qq.com
 */

#include "LZTSPSRequirementLayer.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSSpriteLayer.h"
#include "Based/LZGlobalVariable.h"
#include "Based/LZPlayMusic.h"

TSPSRequriementLayer::TSPSRequriementLayer():
	_objiectives(nullptr)
{
}

TSPSRequriementLayer::~TSPSRequriementLayer()
{
}

bool TSPSRequriementLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createShieldLayer(this);
	createDialog();
	createText();
	showButton();

	return true;
}

void TSPSRequriementLayer::createDialog()
{
	_objiectives = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("LevelObjiectives.png");
	_objiectives->setPosition(Director::getInstance()->getWinSize() / 2.0f);
	_objiectives->setScale(2.0f);
	this->addChild(_objiectives);

	showDialogAction(_objiectives);
	createTouchtListener(_objiectives);
}

void TSPSRequriementLayer::createText()
{
	auto objiectivesText = Text::create();
	objiectivesText->setString(GAME_TEXT("模式说明"));
	objiectivesText->setFontName(GAME_FONT_NAME_1);
	objiectivesText->setFontSize(GAME_TEXT_SIZE("模式说明"));
	objiectivesText->setScale(0.5f);
	objiectivesText->setColor(Color3B(0, 255, 255));
	objiectivesText->setPosition(Vec2(_objiectives->getContentSize().width / 2, 245));
	_objiectives->addChild(objiectivesText);

	auto information = Text::create(GAME_TEXT("模式介绍"), GAME_FONT_NAME_1, GAME_TEXT_SIZE("模式介绍"));
	information->setColor(Color3B::BLACK);
	//information->setGlobalZOrder(10);
	information->setTextVerticalAlignment(TextVAlignment::CENTER);
	information->setTextHorizontalAlignment(TextHAlignment::CENTER);
	information->setTextAreaSize(Size(_objiectives->getContentSize().width - 85, 150));
	information->setPosition(Vec2(_objiectives->getContentSize().width / 2.0f, _objiectives->getContentSize().height / 2.0f - 30));
	_objiectives->addChild(information);
}

void TSPSRequriementLayer::showButton()
{
	auto button = Button::create("Continue1.png", "Continue.png", "",cocos2d::ui::Widget::TextureResType::PLIST);
	button->setTitleText(GAME_TEXT("确定"));
	button->setTitleFontName(GAME_FONT_NAME_1);
	button->setTitleFontSize(GAME_TEXT_SIZE("确定"));
	button->setTitleColor(Color3B::YELLOW);
	button->setPosition(Vec2(_objiectives->getContentSize().width / 2.0f, 10));
	button->setOpacity(0);
	button->setScale(0.5f);
	button->runAction(FadeIn::create(0.5f));
	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap");
				break;
			case ui::Widget::TouchEventType::ENDED:
				SPSSpriteLayer::resumeButtonHoverEffect();
				showDeleteDialogAction(_objiectives);
				break;
			}
		});
	_objiectives->addChild(button);
}
