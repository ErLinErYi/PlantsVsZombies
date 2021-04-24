/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.21
 *Email: 2117610943@qq.com
 */

#include "LZNewspaperZombies.h"
#include "Based/LZPlayMusic.h"

NewspaperZombies::NewspaperZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 3;
	_bloodVolume = 200;
	_headShieldVolume = 0;
	_bodyShieldVolume = 600;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 0;
	_currentBodyShieldVolume = 600;
	_speed = 30;
	_currentSpeed = 30;
	_isHaveShield = true;
	_bodyShieldType = ShieldType::PaperBodyShield;
	_zombiesType = ZombiesType::NewspaperZombies;
}

NewspaperZombies* NewspaperZombies::create(Node* node)
{
	NewspaperZombies* newspaperZombies = new (std::nothrow) NewspaperZombies(node);
	if (newspaperZombies && newspaperZombies->init())
	{
		newspaperZombies->autorelease();
		return newspaperZombies;
	}
	CC_SAFE_DELETE(newspaperZombies);
	return nullptr;
}

void NewspaperZombies::createZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 3 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Walk", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("tt_head", "59");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "48");
}

void NewspaperZombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Stand", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("tt_head", "59");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "48");
}

void NewspaperZombies::playZombieSoundEffect()
{
	Zombies::playZombieSoundEffect(rand() % 2 ? "groan3" : "sukhbir3");
}

void NewspaperZombies::zombieInjuredEventUpdate()
{
	if (_isHaveShield)/* 如果有身体护盾 */
	{
		if (_currentBodyShieldVolume <= _bodyShieldVolume * 2.0f / 3.0f) /* 身体护盾一级损伤 */
		{
			setZombieBodyShieldPrimaryInjure("Zombie_screendoor", "49");
		}
		if (_currentBodyShieldVolume <= _bodyShieldVolume / 3.0f)        /* 身体护盾二级损伤 */
		{
			setZombieBodyShieldSecondaryInjure("Zombie_screendoor", "50");
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

void NewspaperZombies::setZombieBodyShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_bodyShieldAnimationId == 3) /* 护盾三级伤害 */
	{
		_zombiesAnimation->setAttachment("Zombie_screendoor", "tt_innerleg_foot3");
		_bodyShieldAnimationId = 4;
		_isHaveShield = false;
		_attackBodySoundEffectType = 0;
		_bodyShieldType = ShieldType::none;

		PlayMusic::playMusic(rand() % 2 ? "newspaper_rarrgh" : "newspaper_rarrgh2");

		_speed == _currentSpeed ? _speed = _currentSpeed = 60 : _speed = 60;
		_zombiesAnimation->setTimeScale(_zombiesAnimation->getTimeScale() * 2.f);
	}
}

void NewspaperZombies::setZombieSecondaryInjure()
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

		zombieLoseHeadAnimation("ZombieNewspaperHead");

		playZombiesFillDownAnimation();
	}
}
