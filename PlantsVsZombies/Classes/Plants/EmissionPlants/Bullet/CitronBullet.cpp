/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.27
 *Email: 2117610943@qq.com
 */

#include "CitronBullet.h"

#include "../PeaShooter.h"
#include "../../Plants.h"
#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"
#include "Scenes/GameScene/GSBackgroundLayer.h"

CitronBullet::CitronBullet(Node* node, int id):
	_animationId(id)
{
	_node = node;
	_attack = 700;
	_bulletType = BulletType::CitronBullet;
}

CitronBullet::~CitronBullet()
{
}

void CitronBullet::createBullet()
{
    bulletInit();
    createShadow();
}

void CitronBullet::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && getBulletIsSameLineWithZombie(zombie) &&  /* 没有被使用 && 僵尸在同一行 */
			getBulletIsEncounterWithZombie(zombie) &&   /* 与僵尸碰撞 */
			zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap()) /* 僵尸没有死亡 && 僵尸进入地图内 */
		{
			if (_animationId)
			{
				if (!zombie->getZombieIsStrikeFly())
				{
					playSoundEffect("ignite");
					zombie->getZombieAnimation()->getChildByName("shadow")->setVisible(false);
					zombie->getZombieAnimation()->stopAllActions();
					zombie->getZombieAnimation()->setAnimation(1, "Zombies_Stand", false);
					zombie->getZombieAnimation()->setAnchorPoint(Vec2(1.f, 1.f));
					zombie->getZombieAnimation()->runAction(Sequence::create(JumpBy::create(1.0f, Vec2(1000, 500 + rand() % 100), 150 + rand() % 100, 1),
						CallFunc::create([zombie]()
							{
								zombie->setZombieVisible(false);
							}), nullptr));
					zombie->getZombieAnimation()->runAction(RepeatForever::create(RotateBy::create(0.1f, 180)));
					zombie->setZombieIsStrikeFly(true);
				}
			}
			else
			{
				playSoundEffect("cherrybomb");

				setBulletOpacity(); /* 子弹消失 */
				attackZombies();    /* 爆炸对僵尸造成伤害 */
				createExplode();    /* 创建爆炸动画 */
				setBulletAttack(0);
				setBulletIsUsed(true);

				break;
			}
		}
	}
}

void CitronBullet::setAnimationId(const int tag)
{
	_animationId = tag;
}

int CitronBullet::getAnimationId()
{
	return _animationId;
}

void CitronBullet::bulletInit()
{
	_bulletAnimation = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("CitronBullet")->second);
	_bulletAnimation->setAnimation(0, _animationId ? "animation1" : "animation", true);
	_bulletAnimation->setPosition(_position + Vec2(0, 70));
	_bulletAnimation->setScale(1.7f);
	_bulletAnimation->setName(_bulletName);
	_bulletAnimation->setLocalZOrder(getZOrder(_position.y));
	_bulletAnimation->runAction(Sequence::create(MoveBy::create(4.0f, Vec2(2000, 0)),
		CallFunc::create([this]()
			{
				_bulletAnimation->setVisible(false);
			}), nullptr));
	_node->addChild(_bulletAnimation);
}

void CitronBullet::createShadow()
{
	/* 创建影子 */
	auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
	shadow->setScale(1.7f);
	shadow->setName("shadow");
	shadow->setPosition(Vec2(0, -35));
	shadow->setOpacity(200);
	shadow->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	_bulletAnimation->addChild(shadow, -1);
}

void CitronBullet::createExplode()
{
	GSBackgroundLayer::backgroundRunAction();
	auto explode = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("CitronBulletExplode")->second);
	explode->setPosition(getBulletPosition() + Vec2(0, 100));
	explode->setAnimation(0, "animation", false);
	explode->setScale(0.7f);
	explode->setColor(Color3B(219, 255, 173));
	explode->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	explode->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([explode]()
		{
			explode->removeFromParent();
		}), nullptr));
	_node->addChild(explode);	
}

void CitronBullet::attackZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap())
		{
			auto at = static_cast<int>((getZombieInExplodeRange(zombie) / 60 * 350 - 350));
			_attack = 700;

			if (at > 0)_attack -= at;
			if (_attack <= 0) continue;
			bulletAttackHurtZombies(zombie);
			zombie->setZombieHurtBlink();
		}
	}
}

float CitronBullet::getZombieInExplodeRange(Zombies* zombie)
{
	/* 僵尸是否在爆炸范围判断 */
	return sqrt(pow(zombie->getZombieAnimation()->getPositionX() - _bulletAnimation->getPositionX(), 2) +
		pow(zombie->getZombieAnimation()->getPositionY() - _bulletAnimation->getPositionY(), 2));
}

bool CitronBullet::getBulletIsSameLineWithZombie(Zombies* zombie)
{
	return fabs(_bulletAnimation->getPositionY() - 68 - zombie->getZombiePositionY()) <= 10 ? true : false;
}
