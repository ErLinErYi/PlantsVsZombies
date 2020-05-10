/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.08
 *Email: 2117610943@qq.com
 */

#include "BucketDoorFlagZombies.h"

BucketDoorFlagZombies::BucketDoorFlagZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 1;
	_attackBodySoundEffectType = 1;
	_bloodVolume = 200;
	_headShieldVolume = 1000;
	_bodyShieldVolume = 1000;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 1000;
	_currentBodyShieldVolume = 1000;
	_speed = 35;
	_currentSpeed = 35;
	_isHaveShield = true;
	_headShieldType = ShieldType::IronHeadShield;
	_bodyShieldType = ShieldType::IronBodyShield;
	_zombiesType = ZombiesType::BucketDoorFlagZombies;
}

BucketDoorFlagZombies* BucketDoorFlagZombies::create(Node* node)
{
	BucketDoorFlagZombies* bucketDoorZombies = new (std::nothrow) BucketDoorFlagZombies(node);
	if (bucketDoorZombies && bucketDoorZombies->init())
	{
		bucketDoorZombies->autorelease();
		return bucketDoorZombies;
	}
	CC_SAFE_DELETE(bucketDoorZombies);
	return nullptr;
}

void BucketDoorFlagZombies::createZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2 + 2);

	setZombieScale();

	createZombieShadow();

	setAttachment();
	_zombiesAnimation->setAnimation(0, "Zombies_Door_Walk", true);
}

void BucketDoorFlagZombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	setAttachment();
	_zombiesAnimation->setAnimation(0, "Zombies_Door_Stand", true);
}

void BucketDoorFlagZombies::playZombieSoundEffect()
{
	Zombies::playZombieSoundEffect(rand() % 2 ? "groan" : "sukhbir");
}

void BucketDoorFlagZombies::setAttachment()
{
	_zombiesAnimation->setAttachment("Zombie_bucket", "Zombie_bucket1");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "Zombie_screendoor1");
	_zombiesAnimation->setAttachment("tt_innerarm_upper", "tt_flag");
	_zombiesAnimation->setAttachment("tt_innerarm_lower", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("tt_innerarm_hand", "tt_innerleg_foot3");
}