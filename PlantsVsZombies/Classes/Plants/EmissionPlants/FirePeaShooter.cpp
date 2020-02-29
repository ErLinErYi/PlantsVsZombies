/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "FirePeaShooter.h"
#include "Bullet/FirePea.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

FirePeaShooter::FirePeaShooter(Node* node)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::FirePeaShooter;
}

FirePeaShooter::~FirePeaShooter()
{
}

FirePeaShooter* FirePeaShooter::create(Node* node)
{
	FirePeaShooter* firePeaShooter = new (std::nothrow) FirePeaShooter(node);
	if (firePeaShooter && firePeaShooter->init())
	{
		firePeaShooter->autorelease();
		return firePeaShooter;
	}
	CC_SAFE_DELETE(firePeaShooter);
	return nullptr;
}
Sprite* FirePeaShooter::createPlantImage()
{
	imageInit("FirePeaShooter", INIT);
	_plantImage->setScale(1.5f);
	return _plantImage;
}

void FirePeaShooter::createPlantAnimation()
{
	_plantAnimation = plantInit("FirePeaShooter", "FirePeaShooter_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.12f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.6f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 15));

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);
}

void FirePeaShooter::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */

		plantAttack(zombie);         /* 植物攻击 */
	}

	plantRecovery("FirePeaShooter_Normal");
}

void FirePeaShooter::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&     /* 植物存活 && 僵尸没有死亡 && 僵尸进入地图 */
		getZombieIsTheFrontOfPlant(zombie) && getZombieIsSameLineWithPlant(zombie))                 /* 僵尸与植物在同一行 && 僵尸在植物的前方 */
	{
		plantEmission("FirePeaShooter_Shoot");
	}
}

void FirePeaShooter::createPea()
{
	_bulletAnimation = new FirePea(_node);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}
