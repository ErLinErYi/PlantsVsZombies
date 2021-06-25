﻿/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.23
 *Email: 2117610943@qq.com
 */

#include "LZSnowZombies.h"

SnowZombies::SnowZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 0;
	_bloodVolume = 1800;
	_headShieldVolume = 0;
	_bodyShieldVolume = 0;
	_currentBloodVolume = 1800;
	_currentHeadShieldVolume = 0;
	_currentBodyShieldVolume = 0;
	_speed = 35;
	_currentSpeed = 35;
	_isHaveShield = false;
	_zombiesType = ZombiesType::SnowZombies;
	_random.seed(_device());
}

SnowZombies* SnowZombies::create(Node* node)
{
	SnowZombies* snowZombies = new (std::nothrow) SnowZombies(node);
	if (snowZombies && snowZombies->init())
	{
		snowZombies->autorelease();
		return snowZombies;
	}
	CC_SAFE_DELETE(snowZombies);
    return nullptr;
}

void SnowZombies::createZombie()
{
	zombieInit("SnowZombies");

	setZombieAnimation("Zombies_Walk");

	createZombieShadow();

	const uniform_real_distribution<float>number(0.f, 0.3f);
	_zombiesAnimation->setScale(1.2f);
	_zombiesAnimation->setTimeScale(0.3 + number(_random));
}

void SnowZombies::createPreviewZombie()
{
	zombieInit("SnowZombies");

	setZombieAnimation("Zombies_Stand");

	createZombieShadow();

	const uniform_real_distribution<float>number(0.f, 0.5f);
	_zombiesAnimation->setScale(1.2f);
	_zombiesAnimation->setTimeScale(0.6 + number(_random));
}

void SnowZombies::playZombieSoundEffect()
{
	Zombies::playZombieSoundEffect(rand() % 2 ? "groan4" : "sukhbir5");
}

bool SnowZombies::getZombieIsEatPlants()
{
	return false;
}

void SnowZombies::playZombiesAshesAnimation()
{
	const uniform_real_distribution<float>number(0.f, 0.4f);
	auto ashes = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("SnowZombies")->second);
	ashes->setPosition(_zombiesAnimation->getPosition());
	ashes->setLocalZOrder(_zombiesAnimation->getLocalZOrder());
	ashes->setAnimation(0, "Zombies_Stand", false);
	ashes->update(0);
	ashes->setTimeScale(0);
	ashes->setScale(1.2f);
	ashes->setColor(Color3B::BLACK);
	_node->addChild(ashes);

	ashes->runAction(Sequence::create(DelayTime::create(1.f), FadeOut::create(0.5f),
		CallFunc::create([ashes]()
		{
			ashes->removeFromParent();
		}), nullptr));

	setZombieAttributeForGameType(ashes);
}

void SnowZombies::zombieInjuredEventUpdate()
{
	if (_currentBloodVolume <= _bloodVolume / 4.0f)  /* 僵尸血量小于一定值，僵尸掉胳膊 */
	{
		setZombiePrimaryInjure();
	}
	if (_currentBloodVolume <= 0)   /* 僵尸血量小于零，僵尸死亡 */
	{
		setZombieSecondaryInjure();
	}
}

void SnowZombies::setZombiePrimaryInjure()
{
	if (_bodyAnimationId == 1) /* 僵尸开始掉胳膊 */
	{
		_zombiesAnimation->setAttachment("Zombie_yeti_outerarm_upper", "Zombie_yeti_outerarm_upper2");
		_zombiesAnimation->setAttachment("Zombie_yeti_outerarm_lower", "th");
		_zombiesAnimation->setAttachment("Zombie_yeti_outerarm_hand", "th");
		_bodyAnimationId = 2;

		zombieLoseArmAnimation("ZombieYetiArm");
	}
}

void SnowZombies::setZombieSecondaryInjure()
{
	if (_bodyAnimationId == 2)
	{
		_zombiesAnimation->setAttachment("Zombie_yeti_head", "th");
		_zombiesAnimation->setAttachment("Zombie_yeti_jaw", "th");
		_zombiesAnimation->setAnimation(0, "Zombies_Die", false);
		_bodyAnimationId = 10;

		zombieLoseHeadAnimation("ZombieYetiHead");

		playZombiesFillDownAnimation();
	}
}