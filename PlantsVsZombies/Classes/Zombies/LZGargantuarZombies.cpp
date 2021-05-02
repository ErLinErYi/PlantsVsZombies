/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.18
 *Email: 2117610943@qq.com
 */

#include "LZGargantuarZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSBackgroundLayer.h"
#include "Based/LZPlayMusic.h"

GargantuarZombies::GargantuarZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 0;
	_bloodVolume = 6000;
	_headShieldVolume = 0;
	_bodyShieldVolume = 0;
	_currentBloodVolume = 6000;
	_currentHeadShieldVolume = 0;
	_currentBodyShieldVolume = 0;
	_speed = 25;
	_currentSpeed = 25;
	_isShow = false;
	_isHaveShield = false;
	_zombiesType = ZombiesType::GargantuarZombies;
}

GargantuarZombies* GargantuarZombies::create(Node* node)
{
	GargantuarZombies* gargantuarZombies = new (std::nothrow) GargantuarZombies(node);
	if (gargantuarZombies && gargantuarZombies->init())
	{
		gargantuarZombies->autorelease();
		return gargantuarZombies;
	}
	CC_SAFE_DELETE(gargantuarZombies);
	return nullptr;
}

void GargantuarZombies::createZombie()
{
	zombieInit("GargantuarZombies");

	const uniform_real_distribution<float>number(0.f, 0.2f);
	setZombieAnimation("Zombies_Walk");
	_zombiesAnimation->setPosition(_zombiesAnimation->getPosition() - Vec2(0, 10));
	_zombiesAnimation->setTimeScale(0.5f + number(_random));
	_zombiesAnimation->setScale(1.25f);

	createZombieShadow();//影子
	_zombiesAnimation->getChildByName("shadow")->setScale(3.5f);
	_zombiesAnimation->getChildByName("shadow")->setPosition(Vec2(10, 28));
}

void GargantuarZombies::createPreviewZombie()
{
	zombieInit("GargantuarZombies");

	setZombieAnimation(0);

	createZombieShadow();//影子
	_zombiesAnimation->getChildByName("shadow")->setScale(3.5f);
	_zombiesAnimation->getChildByName("shadow")->setPosition(Vec2(10, 28));
	_zombiesAnimation->setScale(1.25f);
}

void GargantuarZombies::playZombieSoundEffect()
{
	Zombies::playZombieSoundEffect(rand() % 2 ? "lowgroan" : "lowgroan2");
}

void GargantuarZombies::zombieInjuredEventUpdate()
{
	if (_currentBloodVolume <= 0)   /* 僵尸血量小于零，僵尸死亡 */
	{
		setZombiePrimaryInjure();
	}
}

void GargantuarZombies::setZombiePrimaryInjure()
{
	if (_bodyAnimationId == 1) 
	{
		_bodyAnimationId = 10;
		
		if (_zombiesAnimation->isVisible())
		{
			PlayMusic::playMusic("gargantudeath");
			_zombiesAnimation->setAnimation(0, "Zombies_Die", false);
			_zombiesAnimation->setTimeScale(0.2f);
			setZombieStop();

			zombieLoseHeadAnimation("Zombie_gargantuar_head_redeye");
			playZombiesFillDownAnimation();
		}
	}
}

void GargantuarZombies::playZombiesAshesAnimation()
{
	const uniform_real_distribution<float>number(0.f, 0.4f);
	auto ashes = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("GargantuarZombieCharred")->second);
	ashes->setPosition(_zombiesAnimation->getPosition() + Vec2(0, -15));
	ashes->setLocalZOrder(_zombiesAnimation->getLocalZOrder());
	ashes->setTimeScale(0.4f + number(_random));
	ashes->update(0);
	ashes->setAnimation(0, "animation", false);
	ashes->setScale(1.1f);
	_node->addChild(ashes);

	ashes->runAction(Sequence::create(DelayTime::create(10.f), 
		CallFunc::create([ashes]()
		{
			ashes->removeFromParent();
		}), nullptr));
}

void GargantuarZombies::zombieLoseHeadAnimation(const std::string& name, const float/*scale*/)
{
	if (_isShowLoseLimbsAnimation)/* 僵尸掉头 */
	{
		PlayMusic::playMusic("limbs_pop");

		auto head = Sprite::createWithSpriteFrameName(name + ".png");
		head->setPosition(_zombiesAnimation->getPosition() + Vec2(-60, 220));
		head->setLocalZOrder(_zombiesAnimation->getLocalZOrder() + 1);
		head->setScale(1.7f);
		head->runAction(Sequence::create(Spawn::create(
			Sequence::create(JumpBy::create(0.5f, Vec2(-150 + rand() % 300, -150), rand() % 100 + 66, 1),
				JumpBy::create(0.2f, Vec2(-20 + rand() % 40, 0), rand() % 10 + 10, 1), nullptr),
			RotateBy::create(0.5f, -180 + rand() % 360), nullptr),
			DelayTime::create(2.0f), FadeOut::create(1.f), CallFunc::create([head]() {head->removeFromParent(); }), nullptr));
		_node->addChild(head);

		showZombieShadow(head, 190);
		setZombieAttributeForGameType(head);
	}
}

void GargantuarZombies::playZombiesFillDownAnimation()
{
	const uniform_real_distribution<float>number(0.f, 0.45f);
	_zombiesAnimation->setTimeScale(0.6f + number(_random));
	_zombiesAnimation->setEventListener([&](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "filldown"))
			{
				_currentSpeed = 0; /* 停止运动 */
				PlayMusic::playMusic("gargantuar_thump");
				GSBackgroundLayer::backgroundRunAction();
			}
			if (!strcmp(event->data->name, "die"))
			{
				zombieFadeOutAnimation();
			}
		});
}
