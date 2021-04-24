/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.23
 *Email: 2117610943@qq.com
 */

#include "LZBonesDoor5Zombies.h"

BonesDoor5Zombies::BonesDoor5Zombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 0;
	_bloodVolume = 200;
	_headShieldVolume = 1800;
	_bodyShieldVolume = 2000;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 1800;
	_currentBodyShieldVolume = 2000;
	_speed = 30;
	_currentSpeed = 30;
	_isHaveShield = true;
	_bodyShieldType = ShieldType::StoneBodyShield;
	_headShieldType = ShieldType::BonesHeadShield;
	_zombiesType = ZombiesType::BonesDoor5Zombies;
}

BonesDoor5Zombies* BonesDoor5Zombies::create(Node* node)
{
	BonesDoor5Zombies* bonesDoor5Zombies = new (std::nothrow) BonesDoor5Zombies(node);
	if (bonesDoor5Zombies && bonesDoor5Zombies->init())
	{
		bonesDoor5Zombies->autorelease();
		return bonesDoor5Zombies;
	}
	CC_SAFE_DELETE(bonesDoor5Zombies);
	return nullptr;
}

void BonesDoor5Zombies::createZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 3 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Walk", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "21");
	_zombiesAnimation->setAttachment("tt_head", "58");
	_zombiesAnimation->setAttachment("tt_jaw", "57");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "51");
}

void BonesDoor5Zombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Stand", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "21");
	_zombiesAnimation->setAttachment("tt_head", "58");
	_zombiesAnimation->setAttachment("tt_jaw", "57");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "51");
}

void BonesDoor5Zombies::zombieInjuredEventUpdate()
{
	if (_isHaveShield)/* 如果有护盾 */
	{
		if (_currentBodyShieldVolume <= _bodyShieldVolume * 2.0f / 3.0f) /* 身体护盾一级损伤 */
		{
			setZombieBodyShieldPrimaryInjure("Zombie_screendoor", "22");
		}
		if (_currentBodyShieldVolume <= _bodyShieldVolume / 3.0f)        /* 身体护盾二级损伤 */
		{
			setZombieBodyShieldSecondaryInjure("Zombie_screendoor", "53");
		}
		if (_currentBodyShieldVolume <= 0) /* 身体护盾消失 */
		{
			setZombieBodyShieldThirdInjure("Zombie_screendoor", "tt_innerleg_foot3");
		}

		if (_currentHeadShieldVolume <= _headShieldVolume * 2.0f / 3.0f) /* 头部护盾一级损伤 */
		{
			setZombieHeadShieldPrimaryInjure("Zombie_bucket", "22");
		}
		if (_currentHeadShieldVolume <= _headShieldVolume / 3.0f)       /* 头部护盾二级损伤 */
		{
			setZombieHeadShieldSecondaryInjure("Zombie_bucket", "23");
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

void BonesDoor5Zombies::setZombieBodyShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_bodyShieldAnimationId == 3) /* 护盾三级伤害 */
	{
		_zombiesAnimation->setAttachment("Zombie_screendoor", "tt_innerleg_foot3");
		_bodyShieldAnimationId = 4;

		_attackBodySoundEffectType = 0;
		_bodyShieldType = ShieldType::none;
	}
}

void BonesDoor5Zombies::setZombieHeadShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_headShieldAnimationId == 3) /* 护盾三级伤害 */
	{
		_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
		_headShieldAnimationId = 4;
		_isHaveShield = false;
		_attackHeadSoundEffectType = 0;
		_headShieldType = ShieldType::none;

		/* 僵尸掉护盾 */
		zombieLoseShieldAnimation("Zombie_Spine", 1.f);
	}
}

void BonesDoor5Zombies::setZombieSecondaryInjure()
{
	if (_bodyAnimationId == 2)
	{
		if (!strcmp(_zombieAnimationName.c_str(), "Zombies_Die") && _zombiesAnimation->getOpacity() < 255)
		{
			_zombiesAnimation->setVisible(false);
		}
		if (strcmp(_zombieAnimationName.c_str(), "Zombies_Die"))
		{
			_zombiesAnimation->setAnimation(0, "Zombies_Die", false);
		}

		_zombiesAnimation->setAttachment("tt_head", "tt_innerleg_foot3");
		_zombiesAnimation->setAttachment("tt_jaw", "tt_innerleg_foot3");
		_bodyAnimationId = 10;

		zombieLoseHeadAnimation("ZombieAncientHead", 1.3f);

		playZombiesFillDownAnimation();
	}
}