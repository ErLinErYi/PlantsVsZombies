/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.26
 *Email: 2117610943@qq.com
 */

#include "LmpZombies.h"

LmpZombies::LmpZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 0;
	_animationId = 7;
	_bloodVolume = 200;
	_headShieldVolume = 0;
	_bodyShieldVolume = 0;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 0;
	_currentBodyShieldVolume = 0;
	_speed = 40;
	_currentSpeed = 40;
	_isHaveShield = false;
	_zombiesType = ZombiesType::LmpZombies;
}

LmpZombies* LmpZombies::create(Node* node)
{
	LmpZombies* lmpZombies = new (std::nothrow) LmpZombies(node);
	if (lmpZombies && lmpZombies->init())
	{
		lmpZombies->autorelease();
		return lmpZombies;
	}
	CC_SAFE_DELETE(lmpZombies);
	return nullptr;
}

void LmpZombies::createZombie()
{
	zombieInit("LmpZombies");

	setZombieAnimationName("Zombies_Walk");

	createZombieShadow();

	_zombiesAnimation->setScale(0.8f);

}

void LmpZombies::createPreviewZombie()
{
	zombieInit("LmpZombies");

	setZombieAnimationName("Zombies_Stand");

	createZombieShadow();

	_zombiesAnimation->setScale(0.8f);
}

void LmpZombies::zombieInjuredEventUpdate()
{
	if (_currentBloodVolume <= _bloodVolume / 2.0f)  /* ½©Ê¬ÑªÁ¿Ð¡ÓÚÒ»¶¨Öµ£¬½©Ê¬µô¸ì²² */
	{
		setZombiePrimaryInjure();
	}
	if (_currentBloodVolume <= 0)   /* ½©Ê¬ÑªÁ¿Ð¡ÓÚÁã£¬½©Ê¬ËÀÍö */
	{
		setZombieSecondaryInjure();
	}
}

void LmpZombies::setZombiePrimaryInjure()
{
	if (_animationId <= 7 && _animationId >= 1) /* ½©Ê¬¿ªÊ¼µô¸ì²² */
	{
		_zombiesAnimation->setAttachment("13", "0");
		_zombiesAnimation->setAttachment("4", "0");
		_zombiesAnimation->setAttachment("18", "9");
		_animationId = 8;

		zombieLoseArmAnimation("LmpZombiesArm");
	}
}

void LmpZombies::setZombieSecondaryInjure()
{
	if (_animationId == 8)
	{
		_zombiesAnimation->setAttachment("3", "0");
		_zombiesAnimation->setAttachment("14", "0");
		_zombiesAnimation->setAnimation(1, "Zombies_Die", false);
		_animationId = 10;

		zombieLoseHeadAnimation("ZombieImpHead");

		zombiesFadeOutAnimation();
	}
}
