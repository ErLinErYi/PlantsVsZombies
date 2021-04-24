/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.22
 *Email: 2117610943@qq.com
 */

#include "LZNewspaperBrickZombies.h"
#include "Based/LZPlayMusic.h"

NewspaperBrickZombies::NewspaperBrickZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 1;
	_attackBodySoundEffectType = 3;
	_bloodVolume = 200;
	_headShieldVolume = 2000;
	_bodyShieldVolume = 1000;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 2000;
	_currentBodyShieldVolume = 1000;
	_speed = 30;
	_currentSpeed = 30;
	_isHaveShield = true;
	_bodyShieldType = ShieldType::PaperBodyShield;
	_headShieldType = ShieldType::BrickHeadShield;
	_zombiesType = ZombiesType::NewspaperBrickZombies;
}

NewspaperBrickZombies* NewspaperBrickZombies::create(Node* node)
{
	NewspaperBrickZombies* newspaperBrickZombies = new (std::nothrow) NewspaperBrickZombies(node);
	if (newspaperBrickZombies && newspaperBrickZombies->init())
	{
		newspaperBrickZombies->autorelease();
		return newspaperBrickZombies;
	}
	CC_SAFE_DELETE(newspaperBrickZombies);
	return nullptr;
}

void NewspaperBrickZombies::createZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 3 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Walk", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "0");
	_zombiesAnimation->setAttachment("tt_head", "59");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "54");
}

void NewspaperBrickZombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Stand", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "0");
	_zombiesAnimation->setAttachment("tt_head", "59");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "54");
}

void NewspaperBrickZombies::zombieInjuredEventUpdate()
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

		if (_currentHeadShieldVolume <= _headShieldVolume * 2.0f / 3) /* 头部护盾一级损伤 */
		{
			setZombieHeadShieldPrimaryInjure("Zombie_bucket", "1");
		}
		if (_currentHeadShieldVolume <= _headShieldVolume * 1.0f / 3) /* 头部护盾二级损伤 */
		{
			setZombieHeadShieldSecondaryInjure("Zombie_bucket", "2");
		}
		if (_currentHeadShieldVolume <= 0)   /* 头部护盾消失 */
		{
			setZombieHeadShieldThirdInjure("Zombie_bucket", "tt_innerleg_foot3");
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

void NewspaperBrickZombies::setZombieBodyShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_bodyShieldAnimationId == 3) /* 护盾三级伤害 */
	{
		_zombiesAnimation->setAttachment("Zombie_screendoor", "tt_innerleg_foot3");
		_bodyShieldAnimationId = 4;
		
		_attackBodySoundEffectType = 0;
		_bodyShieldType = ShieldType::none;

		PlayMusic::playMusic(rand() % 2 ? "newspaper_rarrgh" : "newspaper_rarrgh2");
		
		_speed == _currentSpeed ? _speed = _currentSpeed = 60 : _speed = 60;
		_zombiesAnimation->setTimeScale(_zombiesAnimation->getTimeScale() * 2.f);
	}
}

void NewspaperBrickZombies::setZombieHeadShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_headShieldAnimationId == 3) /* 头部护盾三级伤害 */
	{
		_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
		_headShieldAnimationId = 4;
		_isHaveShield = false;
		_attackHeadSoundEffectType = 0;
		_headShieldType = ShieldType::none;

		/* 僵尸掉头部护盾 */
		zombieLoseShieldAnimation("Zombie_Brick", 1.f);
	}
}
