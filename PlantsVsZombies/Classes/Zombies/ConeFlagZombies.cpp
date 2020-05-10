/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.08
 *Email: 2117610943@qq.com
 */

#include "ConeFlagZombies.h"

ConeFlagZombies::ConeFlagZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 2;
	_attackBodySoundEffectType = 0;
	_bloodVolume = 200;
	_headShieldVolume = 500;
	_bodyShieldVolume = 0;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 500;
	_currentBodyShieldVolume = 0;
	_speed = 40;
	_currentSpeed = 40;
	_isHaveShield = true;
	_headShieldType = ShieldType::PlasticsHeadShield;
	_zombiesType = ZombiesType::ConeFlagZombies;
}

ConeFlagZombies* ConeFlagZombies::create(Node* node)
{
	ConeFlagZombies* coneZombies = new (std::nothrow) ConeFlagZombies(node);
	if (coneZombies && coneZombies->init())
	{
		coneZombies->autorelease();
		return coneZombies;
	}
	CC_SAFE_DELETE(coneZombies);
	return nullptr;
}

void ConeFlagZombies::createZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "Zombie_cone1");
	_zombiesAnimation->setAttachment("tt_innerarm_upper", "tt_flag");
	_zombiesAnimation->setAttachment("tt_innerarm_lower", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("tt_innerarm_hand", "tt_innerleg_foot3");
}

void ConeFlagZombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "Zombie_cone1");
	_zombiesAnimation->setAttachment("tt_innerarm_upper", "tt_flag");
	_zombiesAnimation->setAttachment("tt_innerarm_lower", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("tt_innerarm_hand", "tt_innerleg_foot3");
}

void ConeFlagZombies::playZombieSoundEffect()
{
	Zombies::playZombieSoundEffect(rand() % 2 ? "groan5" : "sukhbir5");
}