/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.14
 *Email: 2117610943@qq.com
 */

#include "LZBMSPSBackgroundLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"
#include "Zombies/LZZombies-Files.h"
#include "Zombies/LZZombies.h"
#include "Based/LZLevelData.h"

BMSPSBackgroundLayer::BMSPSBackgroundLayer()
{
}

BMSPSBackgroundLayer::~BMSPSBackgroundLayer()
{
}

bool BMSPSBackgroundLayer::init()
{
	if (!Layer::init()) return false;

	/* ´´½¨±³¾° */
	createBackGroundEffect();
	showPreviewZombies();

	setBackgroundImagePosition();

	return true;
}

void BMSPSBackgroundLayer::createBackGroundEffect()
{
	_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background8.png"));
}

void BMSPSBackgroundLayer::showPreviewZombies()
{
	SPSBackgroundLayer::createMordernPreviewZombies();
	
	for (auto& p : _previewZombies)
	{
		p.animation->setScale(0.5f);
	}
}