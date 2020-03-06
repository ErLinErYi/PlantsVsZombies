/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "Spikeweed.h"

#include "../EmissionPlants/Bullet/Bullet.h"
#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

Spikeweed::Spikeweed(Node* node):
	_isAttack(false)
,   _isHaveZombies(false)
,   _isChanged(false)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 20;
	_plantsType = PlantsType::Spikeweed;
}

Spikeweed::~Spikeweed()
{
}

Spikeweed* Spikeweed::create(Node* node)
{
	Spikeweed* spikeweed = new (std::nothrow) Spikeweed(node);
	if (spikeweed && spikeweed->init())
	{
		spikeweed->autorelease();
		return spikeweed;
	}
	CC_SAFE_DELETE(spikeweed);
	return nullptr;
}
Sprite* Spikeweed::createPlantImage()
{
	imageInit("Spikeweed", INIT);
	_plantImage->setScale(1.0f);
	return _plantImage;
}

void Spikeweed::createPlantAnimation()
{
	_plantAnimation = plantInit("Spikeweed", "Spikeweed_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(0.8f);
	_node->addChild(_plantAnimation);

	// 影子
	setPlantShadow(1.0f);

	// 泥土飞溅动画
	setPlantSoilSplashAnimation(0.8f);
}

void Spikeweed::determineRelativePositionPlantsAndZombies()
{
	plantAttack();         /* 植物攻击 */

	// 僵尸对植物进行伤害？？？ 在此添加
}

void Spikeweed::plantAttack()
{
	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsSurvive() && getZombieIsSameLineWithPlant(zombie) && getzombieIsEncounterPlant(zombie))       /* 僵尸没有死亡 && 僵尸与植物在同一行 && 僵尸遇到植物 */
		{
			plantAnimationChange(zombie);
		}
	}

	plantAnimationRecovery();
}

void Spikeweed::plantAnimationChange(Zombies* zombie)
{
	_isHaveZombies = true; /* 有僵尸标记 */
	if (!_isChanged)       /* 判断动画是否已经改变 */
	{
		_plantAnimation->addAnimation(0, "Spikeweed_Attack", true);
		_isChanged = true;
	}

	_plantAnimation->setEventListener([this](spTrackEntry* entry, spEvent* event)
		{
			if (strcmp(event->data->name, "Attack") == 0)
			{
				_isAttack = false;
			}
		});

	if (!_isAttack)
	{
		Bullet::selectSoundEffect(zombie->getZombieBodyAttackSoundEffect(), zombie->getZombieHeadAttackSoundEffect());

		zombie->setZombieHurtBlink();

		hurtZombies(zombie);
	}
}

void Spikeweed::plantAnimationRecovery()
{
	if (!_isHaveZombies && _isChanged) /* 没有僵尸 */
	{
		_plantAnimation->addAnimation(0, "Spikeweed_Normal", true);
		_isChanged = false;
	}
	_isHaveZombies = false;
	_isAttack = true;
}
