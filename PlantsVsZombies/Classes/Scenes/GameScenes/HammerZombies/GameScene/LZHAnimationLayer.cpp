﻿/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */

#include "LZHAnimationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSControlLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSZombiesAppearControl.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Based/LZUserData.h"
#include "Based/LZCar.h"
#include "Based/LZPlayMusic.h"
#include "Zombies/LZZombies.h"

HAnimationLayer::HAnimationLayer()
{
}

HAnimationLayer::~HAnimationLayer()
{
}

bool HAnimationLayer::init()
{
	if (!Layer::init())return false;

	if (UserData::getInstance()->openIntUserData(const_cast<char*>("HAMMERZOMBIES_LEVEL_NUMBER")) >= 50)showCars();

	schedule([&](float delta) { gameMainLoop(delta); }, "gameMainLoop");
	schedule([&](float) { coinDeleteUpdate(); }, 2.0f, "coinDeleteUpdate");

	return true;
}

void HAnimationLayer::gameMainLoop(float delta)
{
	zombiesEventUpdate(delta); /* 僵尸事件更新 */
	carsEventUpdate();         /* 小车事件更新 */
}

void HAnimationLayer::zombiesEventUpdate(float delta)
{
	for (auto zombie = ZombiesGroup.begin(); zombie != ZombiesGroup.end();)
	{
		(*zombie)->setZombieMove(delta);
		(*zombie)->zombieInjuredEventUpdate();
		(*zombie)->playZombieSoundEffect();
		Zombies::zombiesDeleteUpdate(zombie);
	}
}

void HAnimationLayer::showCars()
{
	if (!_global->userInformation->getIsReadFileData())
	{
		for (int i = 0; i < 5; ++i)
		{
			this->runAction(Sequence::create(DelayTime::create(0.5f + 0.1 * i), CallFunc::create([=]()
				{
					PlayMusic::playMusic("plastichit2");

					auto car = new Car(this);
					car->setPosition(Vec2(490, 180 + 138 * i));
					car->setInRow(i);
					car->setScale(0.8f);
					car->showCar(CarType::FutureCar);

					CarsGroup.emplace_back(car);
				}), nullptr));
		}
	}
}

void HAnimationLayer::createZombies(const float mulSpeed, const float addSpeed, const unsigned typeNumber)
{
	uniform_int_distribution<unsigned>number(0, 500);
	uniform_int_distribution<unsigned>zombiesType(1, typeNumber);
	auto zombies = createDifferentZombies(static_cast<ZombiesType>(zombiesType(_random)), this);
	const auto row = controlLayerInformation->_zombiesAppearControl->getEqualProbabilityForRow();
	zombies->setZombiePosition(Vec2(1780 + number(_random), ZombiesAppearControl::zombiesPosition[row]));
	zombies->setZombieInRow(row);
	zombies->setZombieAttributeForGameTypeInvalid(true);
	zombies->createZombie();
	zombies->setZombieCurrentSpeed(zombies->getZombieCurrentSpeed() * mulSpeed + addSpeed);
	zombies->getZombieAnimation()->setTimeScale(zombies->getZombieAnimation()->getTimeScale() * mulSpeed + addSpeed);
	zombies->setZombieDieRewardCoinPrecent(40);
	ZombiesGroup.emplace_back(zombies);
	Zombies::zombiesNumbersChange("++");
}
