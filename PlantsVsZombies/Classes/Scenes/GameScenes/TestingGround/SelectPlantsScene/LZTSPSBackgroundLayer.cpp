/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.07
 *Email: 2117610943@qq.com
 */

#include "LZTSPSBackgroundLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSBackgroundLayer.h"
#include "Zombies/LZZombies.h"

TSPSBackgroundLayer::TSPSBackgroundLayer()
{
}

TSPSBackgroundLayer::~TSPSBackgroundLayer()
{
}

bool TSPSBackgroundLayer::init()
{
	if (!Layer::init()) return false;

	/* 创建背景 */
	createBackGroundEffect();
	showPreviewZombies();

	SPSBackgroundLayer::setBackgroundImagePosition();

	return true;
}

void TSPSBackgroundLayer::createBackGroundEffect()
{
	_global->userInformation->setBackground(Sprite::createWithSpriteFrameName("background9.png"));
}

void TSPSBackgroundLayer::showPreviewZombies()
{
	const uniform_int_distribution<unsigned>n(0, 2);
	uniform_int_distribution<unsigned>n1(0, 830);
	uniform_int_distribution<unsigned>n2(0, 830);
	
	for (unsigned int i = 0; i < ZOMBIESNUMBERS; ++i)
	{
		auto zombies = GSAnimationLayer::createDifferentZombies(static_cast<ZombiesType>(i + 1), this);
		zombies->setZombiePosition(getPreviewZombiesPosition(n1, n2));
		zombies->setZombieTag(_previewZombiesTag++);
		zombies->createPreviewZombie();
		zombies->setZombieOpacity(255);

		PreviewZombies previewzombies{};
		previewzombies.animation = zombies->getZombieAnimation();
		previewzombies.position_y = previewzombies.animation->getPositionY();
		previewzombies.position_x = previewzombies.animation->getPositionX();
		_previewZombies.emplace_back(previewzombies);
	}

	sortZombiesPosition();
}
