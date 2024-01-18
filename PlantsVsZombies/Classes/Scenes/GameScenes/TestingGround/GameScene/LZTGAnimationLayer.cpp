﻿/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#include "LZTGAnimationLayer.h"
#include "LZTGGameOverLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSControlLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSZombiesAppearControl.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseQuitLayer.h"
#include "Based/LZCar.h"
#include "Based/LZPlayMusic.h"
#include "Zombies/LZZombies.h"

TGAnimationLayer::TGAnimationLayer(Node* node)
{
	_gameScene = node;
}

TGAnimationLayer::~TGAnimationLayer()
{
}

TGAnimationLayer* TGAnimationLayer::create(Node* node)
{
	TGAnimationLayer* object = new (std::nothrow) TGAnimationLayer(node);
	if (object && object->init())
	{
		object->autorelease();
		return object;
	}
	CC_SAFE_DELETE(object);
	return nullptr;
}

bool TGAnimationLayer::init()
{
	if (!Layer::init())return false;

	showCars();

	schedule([&](float delta) { gameMainLoop(delta); }, "zombiesMainLoop");
	schedule([&](float delta) { gameMainLoop(); }, 1.f / 30.f, "gameMainLoop");
	
	return true;
}

void TGAnimationLayer::zombiesEventUpdate(float delta)
{
	for (auto zombie = ZombiesGroup.begin(); zombie != ZombiesGroup.end();)
	{
		(*zombie)->setZombieMove(delta);
		(*zombie)->zombieInjuredEventUpdate();
		(*zombie)->playZombieSoundEffect();
		judgeZombiesWin(zombie);
		Zombies::zombiesDeleteUpdateNotRecordDieNumbers(zombie);
	}
}

void TGAnimationLayer::showCars()
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
					car->showCar(CarType::WildWestCar);

					CarsGroup.emplace_back(car);
				}), nullptr));
		}
	}
}

void TGAnimationLayer::createZombies(const unsigned typeNumber)
{
	unsigned type = min(static_cast<int>(typeNumber + 1), ZOMBIESNUMBERS);

	uniform_int_distribution<unsigned>number(0, 500);
	uniform_int_distribution<unsigned>zombiesType(1, type);
	auto zombies = createDifferentZombies(static_cast<ZombiesType>(zombiesType(_random)), this);
	const auto row = controlLayerInformation->_zombiesAppearControl->getEqualProbabilityForRow();
	zombies->setZombiePosition(Vec2(1780 + number(_random), ZombiesAppearControl::zombiesPosition[row]));
	zombies->setZombieInRow(row);
	zombies->createZombie();
	zombies->setZombieAttributeForGameTypeInvalid(true);
	ZombiesGroup.emplace_back(zombies);
	Zombies::zombiesNumbersChange("++");
}

void TGAnimationLayer::judgeZombiesWin(list<Zombies*>::iterator& zombie)
{
	if ((*zombie)->getZombieAnimation()->getPositionX() < GRASS_POSITION_LEFT - 80)
	{
		GSPauseQuitLayer::pauseLayer();
		Director::getInstance()->getScheduler()->setTimeScale(1.0f); /* 恢复播放倍数 */
		Director::getInstance()->getOpenGLView()->setCursor("resources/Images/System/cursor.png", Point::ANCHOR_TOP_LEFT);
		
		auto gameOver = TGGameOverLayer::create();
		gameOver->setGlobalZOrder(1);
		Director::getInstance()->getRunningScene()->addChild(gameOver);
	}
}
