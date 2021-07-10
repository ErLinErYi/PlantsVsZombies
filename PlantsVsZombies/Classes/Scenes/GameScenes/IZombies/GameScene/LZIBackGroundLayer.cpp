/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */

#include "LZIBackGroundLayer.h"

IBackgroundLayer::IBackgroundLayer()
{
}

IBackgroundLayer::~IBackgroundLayer()
{
}

bool IBackgroundLayer::init()
{
	if (!Layer::init()) return false;

	_global->userInformation->setSelectWorldName(WorldName::None);
	_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background5.png"));

	setBackgroundImagePosition();

	return true;
}
