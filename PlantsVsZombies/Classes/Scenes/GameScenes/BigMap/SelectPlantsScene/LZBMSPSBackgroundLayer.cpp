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

	/* 创建背景 */
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
		p.animation->setScale(0.8f);
	}

	sortZombiesPosition();
}

Vec2 BMSPSBackgroundLayer::getPreviewZombiesPosition(UID& n1, UID& n2)
{
	Vec2 vec2;
	bool can = false;
	int x, y;
	do
	{
		can = false;
		x = 2250 + n1(_random);
		y = n2(_random) + 40;
		for (const auto& p : _previewZombies)
		{
			if (sqrt(powf(p.position_x - x, 2) + powf(p.position_y - y, 2)) < 100)
			{
				can = true;
				break;
			}
		}
	} while (can);

	vec2.x = x;
	vec2.y = y;

	return vec2;
}