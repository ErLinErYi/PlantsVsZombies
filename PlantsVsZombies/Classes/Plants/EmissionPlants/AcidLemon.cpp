/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.25
 *Email: 2117610943@qq.com
 */

#include "AcidLemon.h"
#include "Bullet/AcidLemonBullet.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

AcidLemonShooter::AcidLemonShooter(Node* node) :
	_isCreateAcidLemon(false)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::AcidLemonShooter;
}

AcidLemonShooter::~AcidLemonShooter()
{
}

AcidLemonShooter* AcidLemonShooter::create(Node* node)
{
	AcidLemonShooter* acidLemonShooter = new (std::nothrow) AcidLemonShooter(node);
	if (acidLemonShooter && acidLemonShooter->init())
	{
		acidLemonShooter->autorelease();
		return acidLemonShooter;
	}
	CC_SAFE_DELETE(acidLemonShooter);
	return nullptr;
}

Sprite* AcidLemonShooter::createPlantImage()
{
	imageInit("LemonShooter", INIT);
	_plantImage->setScale(1.3f);
	return _plantImage;
}

void AcidLemonShooter::createPlantAnimation()
{
	_plantAnimation = plantInit("LemonShooter", "LemonNormal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.2f);
	_plantAnimation->setTimeScale(0.8f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.5f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 10));

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);
}

void AcidLemonShooter::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* 僵尸吃植物 */

		plantAttack(zombie);         /* 植物攻击 */

		zombieRecoveryMove(zombie);  /* 僵尸恢复移动 */
	}

	plantRecovery("LemonNormal");
}

void AcidLemonShooter::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&     /* 植物存活 && 僵尸没有死亡 && 僵尸进入地图 */
		getZombieIsTheFrontOfPlant(zombie) && getZombieIsSameLineWithPlant(zombie))                 /* 僵尸与植物在同一行 && 僵尸在植物的前方 */
	{
		plantEmission("LemonShooter");
	}
}

void AcidLemonShooter::plantEmission(const string& plantAnimation)
{
	_isCreateAcidLemon = true; /* 表示有僵尸与植物在同一行 */
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
				createAcidLemon();
			}
		});
}

void AcidLemonShooter::plantRecovery(const string& plantAnimation)
{
	if (!_isCreateAcidLemon)  /* 判断是否有僵尸与强酸柠檬射手在同一行 */
	{
		if (_isChanged)
		{
			_plantAnimation->addAnimation(0, plantAnimation, true);
			_isChanged = false;
		}
	}

	_isCreateAcidLemon = false; /* 每循环一次就初始化 */
}

void AcidLemonShooter::createAcidLemon()
{
	_bulletAnimation = new AcidLemonBullet(_node);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}