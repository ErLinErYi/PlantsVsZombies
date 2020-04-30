/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.26
 *Email: 2117610943@qq.com
 */

#include "CommonZombies.h"

CommonZombies::CommonZombies(Node* node)
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
	_speed = 30; 
	_currentSpeed = 30;
	_isShow = false;
	_isHaveShield = false;
	_zombiesType = ZombiesType::CommonZombies;
}

CommonZombies* CommonZombies::create(Node* node)
{
	CommonZombies* commonzombies = new (std::nothrow) CommonZombies(node);
	if (commonzombies && commonzombies->init())
	{
		commonzombies->autorelease();
		return commonzombies;
	}
	CC_SAFE_DELETE(commonzombies);
	return nullptr;
}

void CommonZombies::createZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2 + 2);

	setZombieScale();

	createZombieShadow();//影子

	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
}

void CommonZombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
}

void CommonZombies::playZombieSoundEffect()
{
	Zombies::playZombieSoundEffect(rand() % 2 ? "groan6" : "sukhbir6");
}

void CommonZombies::zombieInjuredEventUpdate()
{
	if (_currentBloodVolume < _bloodVolume / 2.0f) /* 僵尸血量小于一定值，僵尸掉胳膊 */
	{
		setZombiePrimaryInjure();
	}
	if (_currentBloodVolume <= 0) /* 僵尸血量小于零，僵尸死亡 */
	{
		setZombieSecondaryInjure();
	}
}
