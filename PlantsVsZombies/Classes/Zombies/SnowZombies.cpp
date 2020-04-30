/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.23
 *Email: 2117610943@qq.com
 */

#include "SnowZombies.h"

SnowZombies::SnowZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 0;
	_bloodVolume = 1600;
	_headShieldVolume = 0;
	_bodyShieldVolume = 0;
	_currentBloodVolume = 1600;
	_currentHeadShieldVolume = 0;
	_currentBodyShieldVolume = 0;
	_speed = 25;
	_currentSpeed = 25;
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

	setZombieAnimationName("Zombies_Walk");

	createZombieShadow();

	uniform_real_distribution<float>number(0.f, 0.3f);
	_zombiesAnimation->setScale(1.2f);
	_zombiesAnimation->setTimeScale(0.3 + number(_random));
}

void SnowZombies::createPreviewZombie()
{
	zombieInit("SnowZombies");

	setZombieAnimationName("Zombies_Stand");

	createZombieShadow();

	uniform_real_distribution<float>number(0.f, 0.5f);
	_zombiesAnimation->setScale(1.2f);
	_zombiesAnimation->setTimeScale(0.6 + number(_random));
}

void SnowZombies::playZombieSoundEffect()
{
	Zombies::playZombieSoundEffect(rand() % 2 ? "groan4" : "sukhbir5");
}

void SnowZombies::zombieInjuredEventUpdate()
{
	if (_currentBloodVolume <= _bloodVolume / 4.0f)  /* ½©Ê¬ÑªÁ¿Ð¡ÓÚÒ»¶¨Öµ£¬½©Ê¬µô¸ì²² */
	{
		setZombiePrimaryInjure();
	}
	if (_currentBloodVolume <= 0)   /* ½©Ê¬ÑªÁ¿Ð¡ÓÚÁã£¬½©Ê¬ËÀÍö */
	{
		setZombieSecondaryInjure();
	}
}

void SnowZombies::setZombiePrimaryInjure()
{
	if (_bodyAnimationId == 1) /* ½©Ê¬¿ªÊ¼µô¸ì²² */
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
		_zombiesAnimation->setAnimation(1, "Zombies_Die", false);
		_bodyAnimationId = 10;

		zombieLoseHeadAnimation("ZombieYetiHead");

		zombieFadeOutAnimation();
	}
}
