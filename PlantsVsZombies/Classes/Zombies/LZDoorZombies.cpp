/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.21
 *Email: 2117610943@qq.com
 */

#include "LZDoorZombies.h"

DoorZombies::DoorZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 3;
	_bloodVolume = 200;
	_headShieldVolume = 0;
	_bodyShieldVolume = 600;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 0;
	_currentBodyShieldVolume = 600;
	_speed = 30;
	_currentSpeed = 30;
	_isHaveShield = true;
	_bodyShieldType = ShieldType::ClothBodyShield;
	_zombiesType = ZombiesType::DoorZombies;
}

DoorZombies* DoorZombies::create(Node* node)
{
	DoorZombies* doorZombies = new (std::nothrow) DoorZombies(node);
	if (doorZombies && doorZombies->init())
	{
		doorZombies->autorelease();
		return doorZombies;
	}
	CC_SAFE_DELETE(doorZombies);
	return nullptr;
}

void DoorZombies::createZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 3 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Walk", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "73");
}

void DoorZombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Stand", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "73");
}

void DoorZombies::playZombieSoundEffect()
{
	Zombies::playZombieSoundEffect(rand() % 2 ? "groan3" : "sukhbir3");
}

void DoorZombies::zombieInjuredEventUpdate()
{
	if (_isHaveShield)/* 如果有身体护盾 */
	{
		if (_currentBodyShieldVolume <= _bodyShieldVolume * 2.0f / 3.0f) /* 身体护盾一级损伤 */
		{
			setZombieBodyShieldPrimaryInjure("Zombie_screendoor", "74");
		}
		if (_currentBodyShieldVolume <= _bodyShieldVolume / 3.0f)        /* 身体护盾二级损伤 */
		{
			setZombieBodyShieldSecondaryInjure("Zombie_screendoor", "75");
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

void DoorZombies::setZombieBodyShieldThirdInjure(const string& oldName, const string& newName)
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
