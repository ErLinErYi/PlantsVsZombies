/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.04
 *Email: 2117610943@qq.com
 */

#include "LZIGameTimerLayer.h"
#include "Scenes/GameScenes/IZombies/GameScene/LZIZombiesData.h"

IGameTimerLayer::IGameTimerLayer()
{
}

IGameTimerLayer::~IGameTimerLayer()
{
}

bool IGameTimerLayer::init()
{
	if (!Layer::init())return false;

	createTimer();

	return true;
}

void IGameTimerLayer::createTimer()
{
	// 每30秒自动存档
	schedule([=](float) {
		IZombiesData::getInstance()->createNewLevelDataDocument();
		IZombiesData::getInstance()->caveLevelData(const_cast<char*>("IZombiesData"));
		}, 30, "caveGameData");

	schedule([this](float) {
		_global->checkAnimationInterval();
		}, 1.f, "FPS");
}
