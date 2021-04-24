/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.23
 *Email: 2117610943@qq.com
 */

#include "LZBrick2Door3Zombies.h"
#include "Based/LZPlayMusic.h"

Brick2Door3Zombies::Brick2Door3Zombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 4;
	_bloodVolume = 200;
	_headShieldVolume = 2000;
	_bodyShieldVolume = 1000;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 2000;
	_currentBodyShieldVolume = 1000;
	_speed = 30;
	_currentSpeed = 30;
	_isHaveShield = true;
	_bodyShieldType = ShieldType::BrickHeadShield;
	_headShieldType = ShieldType::BrickBodyShield;
	_zombiesType = ZombiesType::Brick2Door3Zombies;
}

Brick2Door3Zombies* Brick2Door3Zombies::create(Node* node)
{
	Brick2Door3Zombies* brick2Door3Zombies = new (std::nothrow) Brick2Door3Zombies(node);
	if (brick2Door3Zombies && brick2Door3Zombies->init())
	{
		brick2Door3Zombies->autorelease();
		return brick2Door3Zombies;
	}
	CC_SAFE_DELETE(brick2Door3Zombies);
	return nullptr;
}

void Brick2Door3Zombies::createZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 3 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Walk", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "27");
	_zombiesAnimation->setAttachment("Zombie_screendoor", _doorName[0][_doorNumber]);
}

void Brick2Door3Zombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Stand", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "27");
	_zombiesAnimation->setAttachment("Zombie_screendoor", _doorName[0][_doorNumber]);
}

void Brick2Door3Zombies::zombieInjuredEventUpdate()
{
	if (_isHaveShield)/* 如果有身体护盾 */
	{
		if (_currentBodyShieldVolume <= _bodyShieldVolume * 2.0f / 3.0f) /* 身体护盾一级损伤 */
		{
			setZombieBodyShieldPrimaryInjure("Zombie_screendoor", _doorName[1][_doorNumber]);
		}
		if (_currentBodyShieldVolume <= _bodyShieldVolume / 3.0f)        /* 身体护盾二级损伤 */
		{
			setZombieBodyShieldSecondaryInjure("Zombie_screendoor", _doorName[2][_doorNumber]);
		}
		if (_currentBodyShieldVolume <= 0) /* 身体护盾消失 */
		{
			setZombieBodyShieldThirdInjure("Zombie_screendoor", "tt_innerleg_foot3");
		}

		if (_currentHeadShieldVolume <= _headShieldVolume * 2.0f / 3) /* 头部护盾一级损伤 */
		{
			setZombieHeadShieldPrimaryInjure("Zombie_bucket", "28");
		}
		if (_currentHeadShieldVolume <= _headShieldVolume * 1.0f / 3) /* 头部护盾二级损伤 */
		{
			setZombieHeadShieldSecondaryInjure("Zombie_bucket", "29");
		}
		if (_currentHeadShieldVolume <= 0)   /* 头部护盾消失 */
		{
			setZombieHeadShieldThirdInjure("Zombie_bucket", "tt_innerleg_foot3");
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

void Brick2Door3Zombies::setZombieBodyShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_bodyShieldAnimationId == 3) /* 护盾三级伤害 */
	{
		_zombiesAnimation->setAttachment("Zombie_screendoor", "tt_innerleg_foot3");
		_bodyShieldAnimationId = 4;

		_attackBodySoundEffectType = 0;
		_bodyShieldType = ShieldType::none;
	}
}

void Brick2Door3Zombies::setZombieHeadShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_headShieldAnimationId == 3) /* 头部护盾三级伤害 */
	{
		_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
		_headShieldAnimationId = 4;
		_isHaveShield = false;
		_attackHeadSoundEffectType = 0;
		_headShieldType = ShieldType::none;

		/* 僵尸掉头部护盾 */
		zombieLoseShieldAnimation("Zombie_Brick2", 1.f);
	}
}
