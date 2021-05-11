/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */

#include "LZHBackGroundLayer.h"

HBackgroundLayer::HBackgroundLayer()
{
}

HBackgroundLayer::~HBackgroundLayer()
{
}

bool HBackgroundLayer::init()
{
	if (!Layer::init()) return false;

	_global->userInformation->setSelectWorldName(WorldName::None);
	_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background7.png"));

	setBackgroundImagePosition();

	return true;
}
