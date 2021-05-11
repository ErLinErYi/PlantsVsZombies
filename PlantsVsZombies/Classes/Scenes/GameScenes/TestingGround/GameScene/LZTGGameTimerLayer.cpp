/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.09
 *Email: 2117610943@qq.com
 */

#include "LZTGGameTimerLayer.h"

TGGameTimerLayer::TGGameTimerLayer()
{
}

TGGameTimerLayer::~TGGameTimerLayer()
{
}

bool TGGameTimerLayer::init()
{
	if (!Layer::init())return false;

	createTimer();

	return true;
}

void TGGameTimerLayer::createTimer()
{
	schedule([this](float) {_global->checkAnimationInterval(); }, 1.f, "FPS");
}
