/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.25
 *Email: 2117610943@qq.com
 */

#include "AcidLemonBullet.h"

#include "../PeaShooter.h"
#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

AcidLemonBullet::AcidLemonBullet(Node* node)
{
    _node = node;
    _attack = 15;
    _bulletType = BulletType::AcidLemonBullet;
}

AcidLemonBullet::~AcidLemonBullet()
{
}

void AcidLemonBullet::createBullet()
{
    bulletInit();
    createShadow();
}

void AcidLemonBullet::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && getBulletIsSameLineWithZombie(zombie) &&  /* 没有被使用 && 柠檬酸与僵尸在同一行 */
			getBulletIsEncounterWithZombie(zombie) &&             /* 柠檬酸与僵尸碰撞 */
			zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap()) /* 僵尸没有死亡 && 僵尸进入地图内 */
		{
			selectSoundEffect(zombie->getZombieBodyAttackSoundEffect(), zombie->getZombieHeadAttackSoundEffect());  /* 播放指定音乐 */

			setBulletOpacity();                /* 子弹消失 */
			setAttackForShield(zombie);        /* 对僵尸铁质护盾造成额外伤害 */
			bulletAttackHurtZombies(zombie);   /* 僵尸减少生命值 */

			zombie->setZombieHurtBlink();

			createAcidLemonBulletExplode();   /* 创建爆炸动画 */
			setBulletAttack(0);
			setBulletIsUsed(true);

			break; /* 一个豌豆只能击中一个僵尸 */
		}
	}
}

void AcidLemonBullet::bulletInit()
{
	_bulletAnimation = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("LemonJuice")->second);
	_bulletAnimation->setAnimation(0, rand() % 2 ? "shoot" : "shoot1", true);
	_bulletAnimation->setPosition(_position + Vec2(70, 90));
	_bulletAnimation->setScale(0.9f);
	_bulletAnimation->setName(_bulletName);
	_bulletAnimation->setLocalZOrder(getZOrder(_position.y));
	_node->addChild(_bulletAnimation);
	_bulletAnimation->runAction(Sequence::create(MoveBy::create(2.0f, Vec2(2000, 0)),
		CallFunc::create([this]()
			{
				_bulletAnimation->setVisible(false);
			}), nullptr));
}

void AcidLemonBullet::createShadow()
{
	/* 创建影子 */
	auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
	shadow->setScaleX(1.7f);
	shadow->setName("shadow");
	shadow->setPosition(Vec2(0, -92));
	shadow->setOpacity(200);
	shadow->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	_bulletAnimation->addChild(shadow, -1);
}

void AcidLemonBullet::createAcidLemonBulletExplode()
{
	auto explode = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("PeaExplode")->second);
	explode->setPosition(getBulletPosition());
	explode->setAnimation(0, "Explode4", false);
	explode->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	_node->addChild(explode);

	explode->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([explode]()
		{
			explode->removeFromParent();
		}), nullptr));
}

void AcidLemonBullet::setAttackForShield(Zombies* zombie)
{
	if (zombie->getZombieIsHaveShield()) /* 僵尸有护盾 */
	{
		if (zombie->getZombieHeadShieldType() == ShieldType::IronHeadShield ||
			zombie->getZombieBodyShieldType() == ShieldType::IronBodyShield)     /* 僵尸是铁质护盾 */
		{
			_attack = 80;
		}

		if (zombie->getZombieHeadShieldType() == ShieldType::PlasticsHeadShield) /* 僵尸是塑料护盾 */
		{
			_attack = 15;
		}
	}
	else
	{
		_attack = 20;
	}
}
