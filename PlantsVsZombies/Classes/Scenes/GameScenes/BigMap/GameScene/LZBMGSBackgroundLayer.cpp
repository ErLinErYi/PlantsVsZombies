/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.13
 *Email: 2117610943@qq.com
 */

#include "LZBMGSBackgroundLayer.h"

BMBackgroundLayer::BMBackgroundLayer()
{
}

BMBackgroundLayer::~BMBackgroundLayer()
{
}

bool BMBackgroundLayer::init()
{
	if (!Layer::init()) return false;

	createBackGroundEffect();

	setBackgroundImagePosition();
	return true;
}

void BMBackgroundLayer::createBackGroundEffect()
{
	_global->userInformation->setSelectWorldName(WorldName::None);
	_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background8.png"));
}

void BMBackgroundLayer::setBackgroundImagePosition()
{
	auto background = _global->userInformation->getBackground();
	if (background != nullptr)
	{
		background->setAnchorPoint(Point(0, 0));
		background->setContentSize(Size(5860, 2160));
		background->setPosition(Vec2(0, 0));
		this->addChild(background);
	}
}
