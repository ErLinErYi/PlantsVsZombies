/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.21
 *Email: 2117610943@qq.com
 */

#include "LZDoorZombies3.h"
#include "Based/LZPlayMusic.h"

DoorZombies3::DoorZombies3(Node* node) :
	_doorNumber(rand() % 3),
	_doorName{ {"64","67","70"},{"65","68","71"},{"66","69","72"} }
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 4;
	_bloodVolume = 200;
	_headShieldVolume = 0;
	_bodyShieldVolume = 1000;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 0;
	_currentBodyShieldVolume = 1000;
	_speed = 40;
	_currentSpeed = 40;
	_isHaveShield = true;
	_bodyShieldType = ShieldType::BrickHeadShield;
	_zombiesType = ZombiesType::DoorZombies3;
}

DoorZombies3* DoorZombies3::create(Node* node)
{
	DoorZombies3* doorZombies3 = new (std::nothrow) DoorZombies3(node);
	if (doorZombies3 && doorZombies3->init())
	{
		doorZombies3->autorelease();
		return doorZombies3;
	}
	CC_SAFE_DELETE(doorZombies3);
	return nullptr;
}

void DoorZombies3::createZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 3 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Walk", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("Zombie_screendoor", _doorName[0][_doorNumber]);
}

void DoorZombies3::createPreviewZombie()
{
	zombieInit();

	setZombieAnimation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Stand", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
	_zombiesAnimation->setAttachment("Zombie_screendoor", _doorName[0][_doorNumber]);
}

void DoorZombies3::zombieInjuredEventUpdate()
{
	if (_isHaveShield)/* 如果有身体护盾 */
	{
		if (_currentBodyShieldVolume <= _bodyShieldVolume * 2.0f / 3.0f) /* 身体护盾一级损伤 */
		{
			setZombieBodyShieldPrimaryInjure("Zombie_screendoor", _doorName[1][_doorNumber]);
		}
		if (_currentBodyShieldVolume <= _bodyShieldVolume / 3.0f)        /* 身体护盾二级损伤 */
		{
			setZombieBodyShieldSecondaryInjure("Zombie_screendoor", _doorName[2][_doorNumber]);
		}
		if (_currentBodyShieldVolume <= 0) /* 身体护盾消失 */
		{
			playBodyShieldSpiltAnimation();
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

void DoorZombies3::playBodyShieldSpiltAnimation()
{
	PlayMusic::playMusic("shieldBroken");
	auto animation = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("BrokenTiles")->second);
	animation->setPosition(_zombiesAnimation->getPosition() - Vec2(40, 0));
	animation->setLocalZOrder(_zombiesAnimation->getLocalZOrder());
	animation->setAnimation(0, "animation",false);
	animation->update(0);
	animation->setScale(_zombiesAnimation->getScale());
	_node->addChild(animation);
	animation->runAction(Sequence::create(DelayTime::create(2.5f), 
		CallFunc::create([animation]() 
			{
				animation->removeFromParent();
			}), nullptr));
}
