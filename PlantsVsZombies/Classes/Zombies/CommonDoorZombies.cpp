/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.26
 *Email: 2117610943@qq.com
 */

#include "CommonDoorZombies.h"

CommonDoorZombies::CommonDoorZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 1;
	_bloodVolume = 200;
	_headShieldVolume = 0;
	_bodyShieldVolume = 1200;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 0;
	_currentBodyShieldVolume = 1200;
	_speed = 30;
	_currentSpeed = 30;
	_isHaveShield = true;
	_bodyShieldType = ShieldType::IronBodyShield;
	_zombiesType = ZombiesType::CommonDoorZombies;
}

CommonDoorZombies* CommonDoorZombies::create(Node* node)
{
	CommonDoorZombies* commonDoorZombies = new (std::nothrow) CommonDoorZombies(node);
	if (commonDoorZombies && commonDoorZombies->init())
	{
		commonDoorZombies->autorelease();
		return commonDoorZombies;
	}
	CC_SAFE_DELETE(commonDoorZombies);
	return nullptr;
}

void CommonDoorZombies::createZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Walk", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "Zombie_screendoor1");
}

void CommonDoorZombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Stand", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "Zombie_screendoor1");
}

void CommonDoorZombies::playZombieSoundEffect()
{
	Zombies::playZombieSoundEffect(rand() % 2 ? "groan3" : "sukhbir3");
}

void CommonDoorZombies::zombieInjuredEventUpdate()
{
	if (_isHaveShield)/* 如果有身体护盾 */
	{
		if (_currentBodyShieldVolume <= _bodyShieldVolume * 2.0f / 3.0f) /* 身体护盾一级损伤 */
		{
			setZombieBodyShieldPrimaryInjure("Zombie_screendoor", "Zombie_screendoor2");
		}
		if (_currentBodyShieldVolume <= _bodyShieldVolume / 3.0f)        /* 身体护盾二级损伤 */
		{
			setZombieBodyShieldSecondaryInjure("Zombie_screendoor", "Zombie_screendoor3");
		}
		if (_currentBodyShieldVolume <= 0) /* 身体护盾消失 */
		{
			setZombieBodyShieldThirdInjure("Zombie_screendoor", "tt_innerleg_foot3");
		}
	}
	if (_currentBloodVolume <= _bloodVolume / 2.0f)
	{
		setZombiePrimaryInjure();
	}
	if (_currentBloodVolume <= 0)
	{
		setZombieSecondaryInjure();
	}
}

void CommonDoorZombies::setZombieBodyShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_bodyShieldAnimationId == 3) /* 护盾三级伤害 */
	{
		_zombiesAnimation->setAttachment("Zombie_screendoor", "tt_innerleg_foot3");
		_bodyShieldAnimationId = 4;
		_isHaveShield = false;
		_attackBodySoundEffectType = 0;
		_bodyShieldType = ShieldType::none;
	}
}
