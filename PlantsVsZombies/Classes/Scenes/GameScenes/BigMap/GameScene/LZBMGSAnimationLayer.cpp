/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.14
 *Email: 2117610943@qq.com
 */

#include "Based/LZPlayMusic.h"
#include "Based/LZCar.h"

#include "LZBMGSAnimationLayer.h"
#include "LZBMGSControlLayer.h"

#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSButtonLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSZombiesAppearControl.h"

BMAnimationLayer::BMAnimationLayer(Node* node)
{
}

BMAnimationLayer::~BMAnimationLayer()
{
}

BMAnimationLayer* BMAnimationLayer::create(Node* node)
{
	BMAnimationLayer* object = new (std::nothrow) BMAnimationLayer(node);
	if (object && object->init())
	{
		object->autorelease();
		return object;
	}
	CC_SAFE_DELETE(object);
	return nullptr;
}

bool BMAnimationLayer::init()
{
	if (!Layer::init())return false;

	createRandomSuns();
	showCars();

	schedule([&](float delta) { gameMainLoop(delta); }, "zombiesMainLoop");
	schedule([&](float delta) { gameMainLoop(); }, 1.f / 30.f, "gameMainLoop");
	schedule([&](float) {sunsDeleteUpdate(); coinDeleteUpdate();}, 2.0f, "sunDeleteUpdate");

	return true;
}

void BMAnimationLayer::showCars()
{
	if (!_global->userInformation->getIsReadFileData())
	{
		for (int i = 0; i < 10; ++i)
		{
			this->runAction(Sequence::create(DelayTime::create(0.5f + 0.1 * i), CallFunc::create([=]()
				{
					PlayMusic::playMusic("plastichit2");

					auto car = new Car(this);
					car->setPosition(Vec2(1510, 310 + 136 * i));
					car->setInRow(i);
					car->showCar(CarType::BigMapCar);

					CarsGroup.push_back(car);
				}), nullptr));
		}
	}
}

void BMAnimationLayer::plantPlants()
{
	PlayMusic::playMusic(rand() % 2 == 0 ? "plant2" : "plant");

	auto plants = createDifferentPlants(buttonLayerInformation->mouseSelectImage->selectPlantsId, this);
	plants->setPlantPosition(BIGMAP_ROW_COLUMN_TO_POSITION(controlLayerInformation->_plantsPosition));
	plants->setPlantLocalZOrder(SET_ANIMATION_Z_ORDER(controlLayerInformation->_plantsPosition));
	plants->setPlantRowAndColumn(controlLayerInformation->_plantsPosition);
	plants->setPlantTag(SET_TAG(controlLayerInformation->_plantsPosition));
	plants->createPlantAnimation();

	PlantsGroup.insert(pair<int, Plants*>(plants->getPlantAnimation()->getTag(), plants));
}

void BMAnimationLayer::createZombies()
{
	uniform_int_distribution<unsigned>number(0, 500);
	auto zombies = createDifferentZombies(static_cast<ZombiesType>(
		controlLayerInformation->_zombiesAppearControl->createDifferentTypeZombies(
			controlLayerInformation->_zombiesAppearControl->getZombiesAppearFrequency())), this);
	const auto row = controlLayerInformation->_zombiesAppearControl->getEqualProbabilityForRow(9);
	zombies->setZombiePosition(Vec2(3890 + number(_random), ZombiesAppearControl::zombiesPositionBigMap[row]));
	zombies->setZombieInRow(row);
	zombies->createZombie();
	zombies->setZombieMultipleBloodVolume(4.f);
	ZombiesGroup.push_back(zombies);
	Zombies::zombiesNumbersChange("++");
}

void BMAnimationLayer::zombiesEventUpdate(float delta)
{
	for (auto zombie = ZombiesGroup.begin(); zombie != ZombiesGroup.end();)
	{
		(*zombie)->setZombieMove(delta);
		(*zombie)->zombieInjuredEventUpdate();
		(*zombie)->playZombieSoundEffect();
		BMControlLayer::judgeZombiesWin(*zombie);
		Zombies::zombiesDeleteUpdate(zombie);
	}
}
