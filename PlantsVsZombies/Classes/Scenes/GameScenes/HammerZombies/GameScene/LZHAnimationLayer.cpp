/**
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

	if (UserData::getInstance()->openIntUserData("HAMMERZOMBIES_LEVEL_NUMBER") >= 50)showCars();

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

void HAnimationLayer::createZombies(const float mulSpeed, const float addSpeed, const unsigned typeNumber)
{
	uniform_int_distribution<unsigned>number(0, 500);
	uniform_int_distribution<unsigned>zombiesType(1, typeNumber);
	auto zombies = createDifferentZombies(static_cast<ZombiesType>(zombiesType(_random)), this);
	auto row = controlLayerInformation->_zombiesAppearControl->getEqualProbabilityForRow();
	zombies->setZombiePosition(Vec2(1780 + number(_random), ZombiesAppearControl::zombiesPosition[row]));
	zombies->setZombieInRow(row);
	zombies->setZombieAttributeForGameTypeInvalid(true);
	zombies->createZombie();
	zombies->setZombieCurrentSpeed(zombies->getZombieCurrentSpeed() * mulSpeed + addSpeed);
	zombies->getZombieAnimation()->setTimeScale(zombies->getZombieAnimation()->getTimeScale() * mulSpeed + addSpeed);
	ZombiesGroup.push_back(zombies);
	Zombies::zombiesNumbersChange("++");
}
