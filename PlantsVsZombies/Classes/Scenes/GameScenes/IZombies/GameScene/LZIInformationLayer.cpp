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
	IControlLayer::onSetCurrentLevel(max(1, UserData::getInstance()->openIntUserData(const_cast<char*>("IZOMBIES_LEVEL_NUMBER"))));
	IControlLayer::onSetMostLevel(max(IControlLayer::onGetCurrentLevel(), 
		static_cast<unsigned>(UserData::getInstance()->openIntUserData(const_cast<char*>("IZOMBIES_MOST_LEVEL")))));
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
	username->setString("“" + _global->userInformation->getUserName() + "”- " + GAME_TEXT("我是僵尸").c_str() + " - " +
		StringUtils::format(GAME_TEXT("我是僵尸记录").c_str(), IControlLayer::onGetCurrentLevel(), IControlLayer::onGetMostLevel()) + GAME_TEXT("无尽模式"));
	username->setFontName(GAME_FONT_NAME_1);
	username->setFontSize(GAME_TEXT_SIZE("手指灵活度与忍耐度训练"));
	username->setColor(Color3B(0, 255, 255));
	username->enableGlow(Color4B::ORANGE);
	username->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 950));
	this->addChild(username);
}

void IInformationLayer::showSunNumbers()
{
	GSInformationLayer::showSunNumbers();
	this->getChildByName("SunBank")->setColor(Color3B::MAGENTA);
}
