/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.23
 *Email: 2117610943@qq.com
 */

#include "LZBucket4Door4Zombies.h"
#include "Based/LZPlayMusic.h"

Bucket4Door4Zombies::Bucket4Door4Zombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 1;
	_attackBodySoundEffectType = 1;
	_bloodVolume = 200;
	_headShieldVolume = 1300;
	_bodyShieldVolume = 1600;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 1300;
	_currentBodyShieldVolume = 1600;
	_speed = 35;
	_currentSpeed = 35;
	_isHaveShield = true;
	_bodyShieldType = ShieldType::IronBodyShield;
	_headShieldType = ShieldType::IronHeadShield;
	_zombiesType = ZombiesType::Bucket4Door4Zombies;
}

Bucket4Door4Zombies* Bucket4Door4Zombies::create(Node* node)
{
	Bucket4Door4Zombies* bucket4Door4Zombies = new (std::nothrow) Bucket4Door4Zombies(node);
	if (bucket4Door4Zombies && bucket4Door4Zombies->init())
	{
		bucket4Door4Zombies->autorelease();
		return bucket4Door4Zombies;
	}
	CC_SAFE_DELETE(bucket4Door4Zombies);
	return nullptr;
}

void Bucket4Door4Zombies::createZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 3 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Walk", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "15");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "61");
}

void Bucket4Door4Zombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Stand", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "15");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "61");
}

void Bucket4Door4Zombies::zombieInjuredEventUpdate()
{
	if (_isHaveShield)/* 如果有身体护盾 */
	{
		if (_currentBodyShieldVolume <= _bodyShieldVolume * 2.0f / 3.0f) /* 身体护盾一级损伤 */
		{
			setZombieBodyShieldPrimaryInjure("Zombie_screendoor", "62");
		}
		if (_currentBodyShieldVolume <= _bodyShieldVolume / 3.0f)        /* 身体护盾二级损伤 */
		{
			setZombieBodyShieldSecondaryInjure("Zombie_screendoor", "63");
		}
		if (_currentBodyShieldVolume <= 0) /* 身体护盾消失 */
		{
			setZombieBodyShieldThirdInjure("Zombie_screendoor", "tt_innerleg_foot3");
		}

		if (_currentHeadShieldVolume <= _headShieldVolume * 2.0f / 3) /* 头部护盾一级损伤 */
		{
			setZombieHeadShieldPrimaryInjure("Zombie_bucket", "16");
		}
		if (_currentHeadShieldVolume <= _headShieldVolume * 1.0f / 3) /* 头部护盾二级损伤 */
		{
			setZombieHeadShieldSecondaryInjure("Zombie_bucket", "17");
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

void Bucket4Door4Zombies::setZombieBodyShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_bodyShieldAnimationId == 3) /* 护盾三级伤害 */
	{
		_zombiesAnimation->setAttachment("Zombie_screendoor", "tt_innerleg_foot3");
		_bodyShieldAnimationId = 4;

		_attackBodySoundEffectType = 0;
		_bodyShieldType = ShieldType::none;
	}
}

void Bucket4Door4Zombies::setZombieHeadShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_headShieldAnimationId == 3) /* 头部护盾三级伤害 */
	{
		_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
		_headShieldAnimationId = 4;
		_isHaveShield = false;
		_attackHeadSoundEffectType = 0;
		_headShieldType = ShieldType::none;

		/* 僵尸掉头部护盾 */
		zombieLoseShieldAnimation("Zombie_bucket4", 1.f);
	}
}
