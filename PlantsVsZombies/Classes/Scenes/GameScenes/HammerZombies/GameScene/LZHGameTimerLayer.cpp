/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */

#include "LZHGameTimerLayer.h"

HGameTimerLayer::~HGameTimerLayer()
{
}

HGameTimerLayer::HGameTimerLayer()
{
}

bool HGameTimerLayer::init()
{
	if (!Layer::init())return false;

	createTimer();

	return true;
}

void HGameTimerLayer::createTimer()
{
	schedule([this](float) {_global->checkAnimationInterval();}, 1.f, "FPS");
}
