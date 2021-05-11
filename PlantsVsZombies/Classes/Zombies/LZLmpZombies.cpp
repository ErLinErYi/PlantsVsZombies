/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.26
 *Email: 2117610943@qq.com
 */

#include "LZLmpZombies.h"

LmpZombies::LmpZombies(Node* node)
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
	_isHaveShield = false;
	_zombiesType = ZombiesType::LmpZombies;
	srand(time(nullptr));
}

LmpZombies* LmpZombies::create(Node* node)
{
	LmpZombies* lmpZombies = new (std::nothrow) LmpZombies(node);
	if (lmpZombies && lmpZombies->init())
	{
		lmpZombies->autorelease();
		return lmpZombies;
	}
	CC_SAFE_DELETE(lmpZombies);
	return nullptr;
}

void LmpZombies::createZombie()
{
	zombieInit("LmpZombies");

	setZombieAnimation("Zombies_Walk");

	createZombieShadow();

	_zombiesAnimation->setScale(0.8f);
}

void LmpZombies::createPreviewZombie()
{
	zombieInit("LmpZombies");

	setZombieAnimation("Zombies_Stand");

	createZombieShadow();

	_zombiesAnimation->setScale(0.8f);
}

void LmpZombies::playZombieSoundEffect()
{
	Zombies::playZombieSoundEffect(rand() % 2 ? "imp" : "imp2");
}

void LmpZombies::playZombiesAshesAnimation()
{
	const uniform_real_distribution<float>number(0.f, 0.4f);
	auto ashes = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("LmpZombies_Charre")->second);
	ashes->setPosition(_zombiesAnimation->getPosition() + Vec2(0, -15));
	ashes->setLocalZOrder(_zombiesAnimation->getLocalZOrder());
	ashes->setScale(1.2f);
	ashes->setTimeScale(0.8f + number(_random));
	ashes->setAnimation(0, "animation", false);
	ashes->update(0);
	_node->addChild(ashes);

	ashes->setEventListener([ashes](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "Finished"))
			{
				ashes->runAction(Sequence::create(FadeOut::create(1.0f), CallFunc::create([ashes]()
					{
						ashes->setVisible(false);
					}), nullptr));
			}
		});

	ashes->runAction(Sequence::create(DelayTime::create(3), CallFunc::create([ashes]()
		{
			ashes->removeFromParent();
		}), nullptr));

	setZombieAttributeForGameType(ashes);
}

void LmpZombies::zombieInjuredEventUpdate()
{
	if (_currentBloodVolume <= _bloodVolume / 2.0f)  /* 僵尸血量小于一定值，僵尸掉胳膊 */
	{
		setZombiePrimaryInjure();
	}
	if (_currentBloodVolume <= 0)   /* 僵尸血量小于零，僵尸死亡 */
	{
		setZombieSecondaryInjure();
	}
}

void LmpZombies::setZombiePrimaryInjure()
{
	if (_bodyAnimationId == 1) /* 僵尸开始掉胳膊 */
	{
		_zombiesAnimation->setAttachment("13", "0");
		_zombiesAnimation->setAttachment("4", "0");
		_zombiesAnimation->setAttachment("18", "9");
		_bodyAnimationId = 2;

		zombieLoseArmAnimation("LmpZombiesArm");
	}
}

void LmpZombies::setZombieSecondaryInjure()
{
	if (_bodyAnimationId == 2)
	{
		_zombiesAnimation->setAttachment("3", "0");
		_zombiesAnimation->setAttachment("14", "0");
		_zombiesAnimation->setAnimation(0, "Zombies_Die", false);
		_bodyAnimationId = 10;

		zombieLoseHeadAnimation("ZombieImpHead");

		playZombiesFillDownAnimation();
	}
}
