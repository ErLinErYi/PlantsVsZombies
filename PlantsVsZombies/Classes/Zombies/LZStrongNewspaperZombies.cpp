/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.21
 *Email: 2117610943@qq.com
 */

#include "LZStrongNewspaperZombies.h"

StrongNewspaperZombies::StrongNewspaperZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 3;
	_bloodVolume = 200;
	_headShieldVolume = 0;
	_bodyShieldVolume = 1000;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 0;
	_currentBodyShieldVolume = 1000;
	_speed = 40;
	_currentSpeed = 40;
	_isHaveShield = true;
	_bodyShieldType = ShieldType::PaperBodyShield;
	_zombiesType = ZombiesType::StrongNewspaperZombies;
}

StrongNewspaperZombies* StrongNewspaperZombies::create(Node* node)
{
	StrongNewspaperZombies* strongNewspaperZombies = new (std::nothrow) StrongNewspaperZombies(node);
	if (strongNewspaperZombies && strongNewspaperZombies->init())
	{
		strongNewspaperZombies->autorelease();
		return strongNewspaperZombies;
	}
	CC_SAFE_DELETE(strongNewspaperZombies);
	return nullptr;
}

void StrongNewspaperZombies::createZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 3 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Walk", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("tt_head", "59");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "54");
}

void StrongNewspaperZombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Stand", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("tt_head", "59");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "54");
}

Sprite* StrongNewspaperZombies::createPreviewZombieImage()
{
	imageInit("NewPaperZombie", INIT);
	_zombieImage->setScale(2.f);
	_zombieImage->setAnchorPoint(Vec2(0.7f, 0.3f));
	return _zombieImage;
}

void StrongNewspaperZombies::zombieInjuredEventUpdate()
{
	if (_isHaveShield)/* 如果有身体护盾 */
	{
		if (_currentBodyShieldVolume <= _bodyShieldVolume * 2.0f / 3.0f) /* 身体护盾一级损伤 */
		{
			setZombieBodyShieldPrimaryInjure("Zombie_screendoor", "55");
		}
		if (_currentBodyShieldVolume <= _bodyShieldVolume / 3.0f)        /* 身体护盾二级损伤 */
		{
			setZombieBodyShieldSecondaryInjure("Zombie_screendoor", "56");
		}
		if (_currentBodyShieldVolume <= 0) /* 身体护盾消失 */
		{
			setZombieBodyShieldThirdInjure("Zombie_screendoor", "tt_innerleg_foot3");
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