/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */

#include "LZIInformationLayer.h"
#include "LZIControlLayer.h"
#include "Based/LZUserData.h"
#include "Based/LZPlayMusic.h"

IInformationLayer::IInformationLayer() :
	_informationLayer(nullptr)
{
	IControlLayer::currentLevelNumber = max(1, UserData::getInstance()->openIntUserData(const_cast<char*>("IZOMBIES_LEVEL_NUMBER")));
	IControlLayer::mostLevelNumber = max(IControlLayer::currentLevelNumber, 
		static_cast<unsigned>(UserData::getInstance()->openIntUserData(const_cast<char*>("IZOMBIES_MOST_LEVEL"))));
}

IInformationLayer::~IInformationLayer()
{
}

bool IInformationLayer::init()
{
	if (!Layer::init())return false;

	showUserText();
	showCoinNumbers();
	showSunNumbers();

	return true;
}

void IInformationLayer::showUserText()
{
	auto username = cocos2d::ui::Text::create();
	username->setString("“" + _global->userInformation->getUserName() + "”- " +
		_global->userInformation->getGameText().find("我是僵尸")->second->text.c_str() + " - " +
		StringUtils::format(_global->userInformation->getGameText().find("我是僵尸记录")->second->text.c_str(),
			IControlLayer::currentLevelNumber, IControlLayer::mostLevelNumber) + 
		_global->userInformation->getGameText().find("无尽模式")->second->text);
	username->setFontName(GAME_FONT_NAME_1);
	username->setFontSize(_global->userInformation->getGameText().find("手指灵活度与忍耐度训练")->second->fontsize);
	username->setColor(Color3B(0, 255, 255));
	username->enableGlow(Color4B::ORANGE);
	username->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 950));
	this->addChild(username);
}