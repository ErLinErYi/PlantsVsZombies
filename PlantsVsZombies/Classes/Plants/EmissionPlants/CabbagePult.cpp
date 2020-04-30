/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Emal: 2117610943@qq.com
 */

#include "CabbagePult.h"
#include "PeaShooter.h"
#include "Bullet/Cabbage.h"
#include "Bullet/Bullet.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

#define _MAX_ 0xffffff

CabbagePult::CabbagePult(Node* node):
    _isCreateCabbage(false)
,   _zombiePostion(Vec2::ZERO)
,   _zombie(nullptr)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0; // !!!暂时没有使用 使用的是cabbage里面的_attack
	_plantsType = PlantsType::CabbagePult;
}

CabbagePult::~CabbagePult()
{
}

CabbagePult* CabbagePult::create(Node* node)
{
	CabbagePult* cabbagePult = new (std::nothrow) CabbagePult(node);
	if (cabbagePult && cabbagePult->init())
	{
		cabbagePult->autorelease();
		return cabbagePult;
	}
	CC_SAFE_DELETE(cabbagePult);
	return nullptr;
}
Sprite* CabbagePult::createPlantImage()
{
	imageInit("Cabbage", INIT);
	_plantImage->setScale(1.5f);
	return _plantImage;
}

void CabbagePult::createPlantAnimation()
{
	_plantAnimation = plantInit("Cabbage", "Cabbage_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(0.8f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(2.6f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(1.0f);
}

void CabbagePult::determineRelativePositionPlantsAndZombies()
{
	_distance = _MAX_; /* 初始化距离变量 */

	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		plantAttack(zombie);         /* 植物攻击 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */
	}

	if (_isCreateCabbage)  /* 如果有僵尸与卷心菜投手在同一行 */
	{
		_zombiePostion = _zombie->getZombieAnimation()->getPosition();
		_zombieSpeed = _zombie->getZombieSpeed();
		_plantAnimation->setEventListener([&](spTrackEntry* entry, spEvent* event)
			{
				if (strcmp(event->data->name, "Shoot") == 0)
				{
					rand() % 2 == 0 ? Bullet::playSoundEffect("throw") : Bullet::playSoundEffect("throw2");
					createCabbage();
				}
			});
	}

	if (!_isCreateCabbage) /* 如果没有僵尸与卷心菜投手在同一行 */
	{
		if (_isChanged)
		{
			_plantAnimation->addAnimation(0, "Cabbage_Normal", true);
			_isChanged = false;
		}
	}

	_isCreateCabbage = false; /* 每循环一次就初始化 */
}

void CabbagePult::plantAttack(Zombies* zombie)
{
	if (zombie->getZombieIsSurvive() && getPlantIsSurvive() && zombie->getZombieIsEnterMap() && /* 僵尸存活 && 植物存活 && 僵尸进入地图 */
		getZombieIsSameLineWithPlant(zombie) && getZombieIsTheFrontOfPlant(zombie))             /* 与植物在同一行 && 僵尸在植物的前方 */
	{
		_isCreateCabbage = true; /* 表示有僵尸与植物在同一行 */
		if (!_isChanged)
		{
			_plantAnimation->addAnimation(0, "Cabbage_Shoot", true);
			_isChanged = true;
		}

		float distance = zombie->getZombieAnimation()->getPositionX() - _plantAnimation->getPositionX();
		if (_distance > distance) /* 僵尸与植物的距离小于上一个僵尸与植物的距离 */
		{
			_distance = distance;
			_zombie = zombie;     /* 记录该僵尸 */
		}
	}
}

void CabbagePult::createCabbage()
{
	_bulletAnimation = new Cabbage(_node);
	_bulletAnimation->setBulletPosition(_position);
	dynamic_cast<Cabbage*>(_bulletAnimation)->setZombiePosition(_zombiePostion);
	dynamic_cast<Cabbage*>(_bulletAnimation)->setZombieSpeed(_zombieSpeed);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}


