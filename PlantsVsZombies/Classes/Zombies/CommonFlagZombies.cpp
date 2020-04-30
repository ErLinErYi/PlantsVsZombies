/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.8
 *Email: 2117610943@qq.com
 */

#include "CommonFlagZombies.h"

CommonFlagZombies::CommonFlagZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 0;
	_bloodVolume = 200;
	_headShieldVolume = 0;
	_bodyShieldVolume = 0;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 0;
	_currentBodyShieldVolume = 0;
	_speed = 45;
	_currentSpeed = 45;
	_isShow = false;
	_isHaveShield = false;
	_zombiesType = ZombiesType::CommonFlagZombies;
}

CommonFlagZombies* CommonFlagZombies::create(Node* node)
{
	CommonFlagZombies* commonzombies = new (std::nothrow) CommonFlagZombies(node);
	if (commonzombies && commonzombies->init())
	{
		commonzombies->autorelease();
		return commonzombies;
	}
	CC_SAFE_DELETE(commonzombies);
	return nullptr;
}

void CommonFlagZombies::createZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2 + 2);

	setZombieScale();

	createZombieShadow();//с╟вс

	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("tt_innerarm_upper", "tt_flag");
	_zombiesAnimation->setAttachment("tt_innerarm_lower", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("tt_innerarm_hand", "tt_innerleg_foot3");
}

void CommonFlagZombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("tt_innerarm_upper", "tt_flag");
	_zombiesAnimation->setAttachment("tt_innerarm_lower", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("tt_innerarm_hand", "tt_innerleg_foot3");
}

void CommonFlagZombies::playZombieSoundEffect()
{
	Zombies::playZombieSoundEffect(rand() % 2 ? "groan6" : "sukhbir6");
}