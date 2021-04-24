/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.21
 *Email: 2117610943@qq.com
 */

#include "LZConeZombies2.h"

ConeZombies2::ConeZombies2(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 2;
	_attackBodySoundEffectType = 0;
	_bloodVolume = 300;
	_headShieldVolume = 700;
	_bodyShieldVolume = 0;
	_currentBloodVolume = 300;
	_currentHeadShieldVolume = 700;
	_currentBodyShieldVolume = 0;
	_speed = 30;
	_currentSpeed = 30;
	_isHaveShield = true;
	_headShieldType = ShieldType::PlasticsHeadShield;
	_zombiesType = ZombiesType::ConeZombies2;
}

ConeZombies2* ConeZombies2::create(Node* node)
{
	ConeZombies2* coneZombies2 = new (std::nothrow) ConeZombies2(node);
	if (coneZombies2 && coneZombies2->init())
	{
		coneZombies2->autorelease();
		return coneZombies2;
	}
	CC_SAFE_DELETE(coneZombies2);
	return nullptr;
}

void ConeZombies2::createZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 3 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "30");
}

void ConeZombies2::createPreviewZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "30");
}

void ConeZombies2::zombieInjuredEventUpdate()
{
	if (_isHaveShield)/* 如果头部有护盾 */
	{
		if (_currentHeadShieldVolume <= _headShieldVolume * 2.0f / 3.0f) /* 头部护盾一级损伤 */
		{
			setZombieHeadShieldPrimaryInjure("Zombie_bucket", "31");
		}
		if (_currentHeadShieldVolume <= _headShieldVolume / 3.0f)       /* 头部护盾二级损伤 */
		{
			setZombieHeadShieldSecondaryInjure("Zombie_bucket", "32");
		}
		if (_currentHeadShieldVolume <= 0)                              /* 头部护盾消失 */
		{
			setZombieHeadShieldThirdInjure("Zombie_bucket", "tt_innerleg_foot3");
		}
	}
	if (_currentBloodVolume <= _bloodVolume / 2.0f)   /* 僵尸血量小于一定值，僵尸掉胳膊 */
	{
		setZombiePrimaryInjure();
	}
	if (_currentBloodVolume <= 0)                     /* 僵尸血量小于零，僵尸死亡 */
	{
		setZombieSecondaryInjure();
	}
}

void ConeZombies2::setZombieHeadShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_headShieldAnimationId == 3) /* 护盾三级伤害 */
	{
		_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
		_headShieldAnimationId = 4;
		_isHaveShield = false;
		_attackHeadSoundEffectType = 0;
		_headShieldType = ShieldType::none;

		/* 僵尸掉护盾 */
		zombieLoseShieldAnimation("Zombie_cone2", 1.f);
	}
}
