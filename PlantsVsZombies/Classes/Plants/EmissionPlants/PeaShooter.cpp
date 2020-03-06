/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "PeaShooter.h"
#include "Bullet/Bullet.h"
#include "Bullet/Pea.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"


PeaShooter::PeaShooter(Node* node):
	_isCreatePea(false)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::PeaShooter;
}

PeaShooter::~PeaShooter()
{
}

PeaShooter* PeaShooter::create(Node* node)
{
	PeaShooter* peaShooter = new (std::nothrow) PeaShooter(node);
	if (peaShooter && peaShooter->init())
	{
		peaShooter->autorelease();
		return peaShooter;
	}
	CC_SAFE_DELETE(peaShooter);
	return nullptr;
}

Sprite* PeaShooter::createPlantImage()
{
	imageInit("PeaShooter", INIT);
	_plantImage->setScale(1.3f);
	return _plantImage;
}

void PeaShooter::createPlantAnimation()
{
	_plantAnimation = plantInit("PeaShooter", "PeaShooter_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.0f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.9f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 15));

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);
}

void PeaShooter::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */

		plantAttack(zombie);         /* 植物攻击 */
	}

	plantRecovery("PeaShooter_Normal");
}

void PeaShooter::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&     /* 植物存活 && 僵尸没有死亡 && 僵尸进入地图 */
		getZombieIsTheFrontOfPlant(zombie) && getZombieIsSameLineWithPlant(zombie))                 /* 僵尸与植物在同一行 && 僵尸在植物的前方 */
	{
		plantEmission("PeaShooter_Shoot");
	}
}

void PeaShooter::plantEmission(const string& plantAnimation)
{
	_isCreatePea = true; /* 表示有僵尸与植物在同一行 */
	if (!_isChanged)     /* 判断动画是否已经改变 */
	{
		_plantAnimation->addAnimation(0, plantAnimation, true);
		_isChanged = true;
	}

	_plantAnimation->setEventListener([&](spTrackEntry* entry, spEvent* event)
		{
			if (strcmp(event->data->name, "shoot") == 0)
			{
				rand() % 2 == 0 ? Bullet::playSoundEffect("throw") : Bullet::playSoundEffect("throw2");
				createPea();
			}
		});
}

void PeaShooter::plantRecovery(const string& plantAnimation)
{
	if (!_isCreatePea) /* 判断是否有僵尸与豌豆射手在同一行 */
	{
		if (_isChanged)
		{
			_plantAnimation->addAnimation(0, plantAnimation, true);
			_isChanged = false;
		}
	}

	_isCreatePea = false; /* 每循环一次就初始化 */
}

void PeaShooter::createPea()
{
	_bulletAnimation= new Pea(_node);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}