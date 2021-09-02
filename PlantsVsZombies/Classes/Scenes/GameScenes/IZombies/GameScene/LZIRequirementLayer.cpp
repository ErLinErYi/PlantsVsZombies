/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.9.02
 *Email: 2117610943@qq.com
 */

#include "LZIRequirementLayer.h"

IRequriementLayer::IRequriementLayer()
{
}

IRequriementLayer::~IRequriementLayer()
{
}

void IRequriementLayer::createText()
{
	auto objiectivesText = Text::create();
	objiectivesText->setString(_global->userInformation->getGameText().find("模式说明")->second->text);
	objiectivesText->setFontName(GAME_FONT_NAME_1);
	objiectivesText->setFontSize(_global->userInformation->getGameText().find("模式说明")->second->fontsize);
	objiectivesText->setScale(0.5f);
	objiectivesText->setColor(Color3B(0, 255, 255));
	objiectivesText->setPosition(Vec2(_objiectives->getContentSize().width / 2, 245));
	_objiectives->addChild(objiectivesText);

	auto str = _global->userInformation->getGameText().find("我是僵尸模式介绍")->second;
	auto information = Text::create(str->text, GAME_FONT_NAME_1, str->fontsize);
	information->setColor(Color3B::BLACK);
	information->setGlobalZOrder(10);
	information->setTextVerticalAlignment(TextVAlignment::CENTER);
	information->setTextHorizontalAlignment(TextHAlignment::CENTER);
	information->setTextAreaSize(Size(_objiectives->getContentSize().width - 85, 150));
	information->setPosition(Vec2(_objiectives->getContentSize().width / 2.0f, _objiectives->getContentSize().height / 2.0f - 30));
	_objiectives->addChild(information);
}