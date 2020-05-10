/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.26
 *Email: 2117610943@qq.com
 */

#include "BucketZombies.h"

BucketZombies::BucketZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 1;
	_attackBodySoundEffectType = 0;
	_bloodVolume = 200;
	_headShieldVolume = 1200;
	_bodyShieldVolume = 0;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 1200;
	_currentBodyShieldVolume = 0;
	_speed = 30;
	_currentSpeed = 30;
	_isHaveShield = true;
	_headShieldType = ShieldType::IronHeadShield;
	_zombiesType = ZombiesType::BucketZombies;
}

BucketZombies* BucketZombies::create(Node* node)
{
	BucketZombies* bucketZombies = new (std::nothrow) BucketZombies(node);
	if (bucketZombies && bucketZombies->init())
	{
		bucketZombies->autorelease();
		return bucketZombies;
	}
	CC_SAFE_DELETE(bucketZombies);
	return nullptr;
}

void BucketZombies::createZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "Zombie_bucket1");
}

void BucketZombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "Zombie_bucket1");
}

void BucketZombies::playZombieSoundEffect()
{
	Zombies::playZombieSoundEffect(rand() % 2 ? "groan4" : "sukhbir4");
}

void BucketZombies::zombieInjuredEventUpdate()
{
	if (_isHaveShield)/* 如果有头部护盾 */
	{
		if (_currentHeadShieldVolume <= _headShieldVolume * 2.0f / 3.0f) /* 头部护盾一级损伤 */
		{
			setZombieHeadShieldPrimaryInjure("Zombie_bucket", "Zombie_bucket2");
		}
		if (_currentHeadShieldVolume <= _headShieldVolume / 3.0f)       /* 头部护盾二级损伤 */
		{
			setZombieHeadShieldSecondaryInjure("Zombie_bucket", "Zombie_bucket3");
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

void BucketZombies::setZombieHeadShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_headShieldAnimationId == 3) /* 护盾三级伤害 */
	{
		_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
		_headShieldAnimationId = 4;
		_isHaveShield = false;
		_attackHeadSoundEffectType = 0;
		_headShieldType = ShieldType::none;

		/* 僵尸掉护盾 */
		zombieLoseShieldAnimation("Zombie_bucket");
	}
}
