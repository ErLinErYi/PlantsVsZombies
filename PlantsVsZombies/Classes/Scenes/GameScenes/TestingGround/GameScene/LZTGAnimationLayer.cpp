/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#include "LZTGAnimationLayer.h"
#include "LZTGGameOverLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSDefine.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSControlLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSZombiesAppearControl.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseQuitLayer.h"
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

void TGAnimationLayer::createZombies(const unsigned typeNumber)
{
	unsigned type = 1;
	if (typeNumber < 1)type = 1;
	else if (typeNumber > 15)type = 15;
	else type = typeNumber;

	uniform_int_distribution<unsigned>number(0, 500);
	uniform_int_distribution<unsigned>zombiesType(1, type);
	auto zombies = createDifferentZombies(static_cast<ZombiesType>(zombiesType(_random)), this);
	auto row = controlLayerInformation->_zombiesAppearControl->getEqualProbabilityForRow();
	zombies->setZombiePosition(Vec2(1780 + number(_random), ZombiesAppearControl::zombiesPosition[row]));
	zombies->setZombieInRow(row);
	zombies->createZombie();
	zombies->setZombieAttributeForGameTypeInvalid(true);
	ZombiesGroup.push_back(zombies);
	Zombies::zombiesNumbersChange("++");
}

void TGAnimationLayer::judgeZombiesWin(list<Zombies*>::iterator& zombie)
{
	if ((*zombie)->getZombieAnimation()->getPositionX() < GRASS_POSITION_LEFT - 80)
	{
		GSPauseQuitLayer::pauseLayer();
		Director::getInstance()->getScheduler()->setTimeScale(1.0f); /* »Ö¸´²¥·Å±¶Êý */
		Director::getInstance()->getOpenGLView()->setCursor("resources/images/System/cursor.png", Point::ANCHOR_TOP_LEFT);
		
		auto gameOver = TGGameOverLayer::create();
		gameOver->setGlobalZOrder(1);
		Director::getInstance()->getRunningScene()->addChild(gameOver);
	}
}
