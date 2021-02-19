/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.15
 *Email: 2117610943@qq.com
 */

#include "LZSBInformationLayer.h"
#include "Based/LZBGameType.h"

BMInformationLayer::BMInformationLayer()
{
}

BMInformationLayer::~BMInformationLayer()
{
}

bool BMInformationLayer::init()
{
	if (!Layer::init())return false;

	showUserText();
	showProgressBar();
	showZombiesDieNumbers();
	showCoinNumbers();
	showSunNumbers();
	showGameType();

	return true;
}

void BMInformationLayer::showGameType()
{
	gameType = new GameType(this);
}

void BMInformationLayer::showUserText()
{
	GSInformationLayer::showUserText();
	this->getChildByName("userName")->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 960));
}
