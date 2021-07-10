/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */

#include "LZIAnimationLayer.h"
#include "LZIOpenCaveGameLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSControlLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSZombiesAppearControl.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Based/LZPlayMusic.h"
#include "Zombies/LZZombies.h"

list<IAnimationLayer::Brain> IAnimationLayer::BrainGroup;

IAnimationLayer::IAnimationLayer()
{
	BrainGroup.clear();
}

IAnimationLayer::~IAnimationLayer()
{
}

bool IAnimationLayer::init()
{
	if (!Layer::init())return false;

	showBrain();
	showFlower();

	schedule([&](float delta) { gameMainLoop(delta); }, "zombiesMainLoop");
	schedule([&](float) { gameMainLoop(); }, 1.f / 30.f, "gameMainLoop");
	schedule([&](float) { coinDeleteUpdate(); sunsDeleteUpdate(); }, 2.0f, "coinDeleteUpdate");

	return true;
}

void IAnimationLayer::gameMainLoop()
{
	plantsEventUpdate();       /* 植物事件更新 */
	bulletEventUpdate();       /* 子弹事件更新 */
}

void IAnimationLayer::gameMainLoop(float delta)
{
	zombiesEventUpdate(delta); /* 僵尸事件更新 */
}

void IAnimationLayer::zombiesEventUpdate(float delta)
{
	for (auto zombie = ZombiesGroup.begin(); zombie != ZombiesGroup.end();)
	{
		(*zombie)->setZombieMove(delta);
		(*zombie)->zombieInjuredEventUpdate();
		(*zombie)->playZombieSoundEffect();
		Zombies::zombiesDeleteUpdateNotRecordDieNumbers(zombie);
	}
}

void IAnimationLayer::createZombies(ZombiesType type, float xPosition)
{
	PlayMusic::playMusic(rand() % 2 == 0 ? "plant2" : "plant");

	auto zombies = createDifferentZombies(type, this);
	zombies->setZombiePosition(Vec2(xPosition, ZombiesAppearControl::zombiesPosition[static_cast<unsigned int>(controlLayerInformation->_plantsPosition.y)]));
	zombies->setZombieInRow(controlLayerInformation->_plantsPosition.y);
	zombies->setZombieAttributeForGameTypeInvalid(true);
	zombies->createZombie();
	ZombiesGroup.push_back(zombies);
	Zombies::zombiesNumbersChange("++");
}

void IAnimationLayer::showBrain()
{
	if (!IOpenCaveGameLayer::isReadFile)
	{
		for (int i = 0; i < 5; ++i)
		{
			auto brain = Sprite::createWithSpriteFrameName("Brain.png");
			brain->setPosition(Vec2(500, 180 + 138 * i));
			brain->setScale(0.3f);
			brain->setLocalZOrder((50 - i) * 100 + 90);
			this->addChild(brain);

			auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
			shadow->setScale(8.0f);
			shadow->setPosition(Vec2(220, 110));
			brain->addChild(shadow, -1);

			Brain b;
			b.brain = brain;
			b.enable = true;
			b.row = i;
			BrainGroup.push_back(b);
		}
	}
}

void IAnimationLayer::showFlower()
{
	auto flowers = Sprite::createWithSpriteFrameName("Flowers.png");
	flowers->setPosition(Vec2(570 + 122 * 5, 600));
	flowers->setScaleY(1.2f);
	this->addChild(flowers);
	auto flowers1 = Sprite::createWithSpriteFrameName("Flowers.png");
	flowers1->setPosition(Vec2(570 + 122 * 5, 300));
	flowers1->setScaleY(1.2f);
	this->addChild(flowers1);
}