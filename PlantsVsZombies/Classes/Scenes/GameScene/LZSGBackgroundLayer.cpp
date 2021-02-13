/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#include "LZSGBackgroundLayer.h"
#include "LZSGData.h"

#include "Based/LZBGameType.h"
#include "Based/LZBLevelData.h"
#include "Zombies/LZZCommonZombies.h"

GSBackgroundLayer::GSBackgroundLayer():
	_global(Global::getInstance()),
	gameType(nullptr)
{
}

GSBackgroundLayer::~GSBackgroundLayer()
{
	if (gameType)delete gameType;
}

void GSBackgroundLayer::backgroundRunAction()
{
	Vec2 begin[4] = { Vec2(2,2),Vec2(-2,2),Vec2(2,-2),Vec2(-2,-2) };
	auto move = MoveBy::create(0.1f, begin[rand() % 4]);
	backgroundLayerInformation->runAction(Sequence::create(move, move->reverse(), move, move->reverse(), nullptr));
}

bool GSBackgroundLayer::init()
{
	if (!Layer::init()) return false;

	createBackGroundEffect();

	setBackgroundImagePosition();

	showGameType();

	return true;
}

void GSBackgroundLayer::setBackgroundImagePosition()
{
	auto background = _global->userInformation->getBackground();
	if (background != nullptr)
	{
		background->setAnchorPoint(Point(0, 0));
		background->setContentSize(Size(2930, 1081));
		background->setPosition(Vec2(-220, 0));
		this->addChild(background);
    }
}

void GSBackgroundLayer::showGameType()
{
	gameType = new GameType(this);
	gameType->createGameType();
}
