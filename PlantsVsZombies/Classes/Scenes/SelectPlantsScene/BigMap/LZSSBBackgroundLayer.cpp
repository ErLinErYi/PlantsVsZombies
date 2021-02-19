/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.14
 *Email: 2117610943@qq.com
 */

#include "LZSSBBackgroundLayer.h"
#include "../../GameScene/LZSGAnimationLayer.h"
#include "../../GameScene/LZSGData.h"
#include "Zombies/LZZZombies-Files.h"
#include "Zombies/LZZZombies.h"
#include "Based/LZBLevelData.h"

SBBackgroundLayer::SBBackgroundLayer()
{
}

SBBackgroundLayer::~SBBackgroundLayer()
{
}

bool SBBackgroundLayer::init()
{
	if (!Layer::init()) return false;

	/* ´´½¨±³¾° */
	createBackGroundEffect();
	showPreviewZombies();

	setBackgroundImagePosition();

	return true;
}

void SBBackgroundLayer::createBackGroundEffect()
{
	_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background8.png"));
}

void SBBackgroundLayer::showPreviewZombies()
{
	SPSBackgroundLayer::createMordernPreviewZombies();
	
	for (auto& p : _previewZombies)
	{
		p.animation->setScale(0.5f);
	}
}