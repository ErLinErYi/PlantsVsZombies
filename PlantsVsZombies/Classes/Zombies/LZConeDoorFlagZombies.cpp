/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.08
 *Email: 2117610943@qq.com
 */

#include "LZConeDoorFlagZombies.h"

ConeDoorFlagZombies::ConeDoorFlagZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 2;
	_attackBodySoundEffectType = 1;
	_bloodVolume = 200;
	_headShieldVolume = 600;
	_bodyShieldVolume = 1200;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 600;
	_currentBodyShieldVolume = 1200;
	_speed = 36;
	_currentSpeed = 36;
	_isHaveShield = true;
	_headShieldType = ShieldType::PlasticsHeadShield;
	_bodyShieldType = ShieldType::IronBodyShield;
	_zombiesType = ZombiesType::ConeDoorFlagZombies;
}

ConeDoorFlagZombies* ConeDoorFlagZombies::create(Node* node)
{
	ConeDoorFlagZombies* coneDoorZombies = new (std::nothrow) ConeDoorFlagZombies(node);
	if (coneDoorZombies && coneDoorZombies->init())
	{
		coneDoorZombies->autorelease();
		return coneDoorZombies;
	}
	CC_SAFE_DELETE(coneDoorZombies);
	return nullptr;
}

void ConeDoorFlagZombies::createZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 3 + 2);

	setZombieScale();

	createZombieShadow();

	setAttachment();
	_zombiesAnimation->setAnimation(0, "Zombies_Door_Walk", true);
}

void ConeDoorFlagZombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	setAttachment();
	_zombiesAnimation->setAnimation(0, "Zombies_Door_Stand", true);
}

void ConeDoorFlagZombies::playZombieSoundEffect()
{
	Zombies::playZombieSoundEffect(rand() % 2 ? "groan2" : "sukhbir2");
}

void ConeDoorFlagZombies::setAttachment()
{
	_zombiesAnimation->setAttachment("Zombie_bucket", "Zombie_cone1");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "Zombie_screendoor1");
	_zombiesAnimation->setAttachment("tt_innerarm_upper", rand() % 4 ? "tt_flag" : "tt_flag1");
	_zombiesAnimation->setAttachment("tt_innerarm_lower", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("tt_innerarm_hand", "tt_innerleg_foot3");
}
