/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#include "LZTGBackgoundLayer.h"

TGBackgroundLayer::TGBackgroundLayer()
{
}

TGBackgroundLayer::~TGBackgroundLayer()
{
}

bool TGBackgroundLayer::init()
{
	if (!Layer::init()) return false;

	createBackGroundEffect();

	setBackgroundImagePosition();
	return true;
}
