/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.08
 *Email: 2117610943@qq.com
 */

#include "BucketFlagZombies.h"

BucketFlagZombies::BucketFlagZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 1;
	_attackBodySoundEffectType = 0;
	_bloodVolume = 200;
	_headShieldVolume = 1000;
	_bodyShieldVolume = 0;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 1000;
	_currentBodyShieldVolume = 0;
	_speed = 40;
	_currentSpeed = 40;
	_isHaveShield = true;
	_headShieldType = ShieldType::IronHeadShield;
	_bodyShieldType = ShieldType::none;
	_zombiesType = ZombiesType::BucketFlagZombies;
}

BucketFlagZombies* BucketFlagZombies::create(Node* node)
{
	BucketFlagZombies* bucketZombies = new (std::nothrow) BucketFlagZombies(node);
	if (bucketZombies && bucketZombies->init())
	{
		bucketZombies->autorelease();
		return bucketZombies;
	}
	CC_SAFE_DELETE(bucketZombies);
	return nullptr;
}

void BucketFlagZombies::createZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "Zombie_bucket1");
	_zombiesAnimation->setAttachment("tt_innerarm_upper", "tt_flag");
	_zombiesAnimation->setAttachment("tt_innerarm_lower", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("tt_innerarm_hand", "tt_innerleg_foot3");
}

void BucketFlagZombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "Zombie_bucket1");
	_zombiesAnimation->setAttachment("tt_innerarm_upper", "tt_flag");
	_zombiesAnimation->setAttachment("tt_innerarm_lower", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("tt_innerarm_hand", "tt_innerleg_foot3");
}

void BucketFlagZombies::playZombieSoundEffect()
{
	Zombies::playZombieSoundEffect(rand() % 2 ? "groan4" : "sukhbir4");
}