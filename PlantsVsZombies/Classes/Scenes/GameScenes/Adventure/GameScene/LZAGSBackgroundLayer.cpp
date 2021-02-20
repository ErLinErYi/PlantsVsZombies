/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#include "LZAGSBackgroundLayer.h"
#include "LZAGSData.h"

#include "Based/LZGameType.h"
#include "Based/LZLevelData.h"
#include "Zombies/LZCommonZombies.h"

GSBackgroundLayer::GSBackgroundLayer():
	_global(Global::getInstance())
{
}

GSBackgroundLayer::~GSBackgroundLayer()
{
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
