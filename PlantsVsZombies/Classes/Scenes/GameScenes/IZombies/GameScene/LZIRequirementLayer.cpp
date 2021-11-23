/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.9.02
 *Email: 2117610943@qq.com
 */

#include "LZIRequirementLayer.h"
#include "Based/LZDefine.h"

IRequriementLayer::IRequriementLayer()
{
}

IRequriementLayer::~IRequriementLayer()
{
}

void IRequriementLayer::createText()
{
	auto objiectivesText = Text::create();
	objiectivesText->setString(GAME_TEXT("模式说明"));
	objiectivesText->setFontName(GAME_FONT_NAME_1);
	objiectivesText->setFontSize(GAME_TEXT_SIZE("模式说明"));
	objiectivesText->setScale(0.5f);
	objiectivesText->setColor(Color3B(0, 255, 255));
	objiectivesText->setPosition(Vec2(_objiectives->getContentSize().width / 2, 245));
	_objiectives->addChild(objiectivesText);

	auto information = Text::create(GAME_TEXT("我是僵尸模式介绍"), GAME_FONT_NAME_1, GAME_TEXT_SIZE("我是僵尸模式介绍"));
	information->setColor(Color3B::BLACK);
	information->setGlobalZOrder(10);
	information->setTextVerticalAlignment(TextVAlignment::CENTER);
	information->setTextHorizontalAlignment(TextHAlignment::CENTER);
	information->setTextAreaSize(Size(_objiectives->getContentSize().width - 85, 150));
	information->setPosition(Vec2(_objiectives->getContentSize().width / 2.0f, _objiectives->getContentSize().height / 2.0f - 30));
	_objiectives->addChild(information);
}