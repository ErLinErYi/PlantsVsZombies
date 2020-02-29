/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.18
 *Email: 2117610943@qq.com
 */

#include "Cabbage.h"
#include "../PeaShooter.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

Cabbage::Cabbage(Node* node) :
	_zombiePosition(Vec2::ZERO)
{
	_node = node;
	_attack = 60;
	_bulletType = BulletType::Cabbage;
}

void Cabbage::createBullet()
{
	bulletInit();
	createShadow();
}

void Cabbage::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && zombie->getZombieIsSurvive() &&                                           /* 卷心菜没有使用 && 僵尸没有死亡 */
			getBulletIsSameLineWithZombie(zombie) && getBulletIsEncounterWithZombie(zombie))      /* 子弹与僵尸同一行 && 子弹与僵尸碰撞 */
		{
			PeaShooter::playSoundEffect(static_cast<SoundEffectType>(zombie->getZombieHeadAttackSoundEffect()));
			_bulletAnimation->setOpacity(0);
			bulletAttackHurtZombies(zombie);   /* 僵尸减少生命值 */

			zombie->getZombieAnimation()->runAction(Sequence::create(TintTo::create(0.2f, Color3B(100, 100, 100)), TintTo::create(0.2f, Color3B::WHITE), nullptr));

			createCabbageExplode();

			_isUsed = true;
			break; /* 一个只能击中一个僵尸 */
		}
	}
}

void Cabbage::bulletInit()
{
	_bulletAnimation = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("CabbageBullet")->second);
	_bulletAnimation->setAnimation(0, "Cabbage_Rotate", true);
	_bulletAnimation->setScale(0.8f);
	_bulletAnimation->setLocalZOrder(getZOrder(_position.y));
	_bulletAnimation->setPosition(_position + Vec2(70, 150));
	_bulletAnimation->setAnchorPoint(Vec2(0, 0));
	_node->addChild(_bulletAnimation);

	_bulletAnimation->runAction(Sequence::create(JumpTo::create(1.0f, _zombiePosition + Vec2(5, 20), 300, 1),
		CallFunc::create([this]()
			{
				if (_bulletAnimation->getOpacity()) /* 如果没有隐藏说明没有击中僵尸 */
				{
					PeaShooter::playSoundEffect(static_cast <SoundEffectType>(0));
				}
				_bulletAnimation->setAnimation(0, "Cabbage_Crush", false);
			}), DelayTime::create(1.4f),
				CallFunc::create([this]()
					{
						_bulletAnimation->runAction(Sequence::create(FadeOut::create(0.2f),
							CallFunc::create([this]()
								{
									_bulletAnimation->setVisible(false);
								}), nullptr));
					}), nullptr));
}

void Cabbage::createShadow()
{
	/* 创建影子 */
	auto shadow = Sprite::create(_global->userInformation->getImagePath().find("plantshadow")->second);
	shadow->setPosition(_position + Vec2(70, 10));
	shadow->setLocalZOrder(getZOrder(_position.y));
	_node->addChild(shadow);
	shadow->runAction(Sequence::create(MoveTo::create(0.9f, Vec2(_zombiePosition.x - 100, _position.y + 15)),
		CallFunc::create([shadow]()
			{
				shadow->removeFromParent();
			}), nullptr));
}

void Cabbage::createCabbageExplode()
{
	auto cabbageExplode = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("CabbageBullet")->second);
	cabbageExplode->setAnimation(0, "Cabbage_Crush", false);
	cabbageExplode->setScale(0.6f);
	cabbageExplode->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	cabbageExplode->setPosition(_bulletAnimation->getPosition());
	cabbageExplode->setAnchorPoint(Vec2(0, 0));
	cabbageExplode->runAction(Sequence::create(DelayTime::create(1.4f),
		CallFunc::create([cabbageExplode]()
			{
				cabbageExplode->runAction(Sequence::create(FadeOut::create(0.2f),
					CallFunc::create([cabbageExplode]()
						{
							cabbageExplode->removeFromParent();
						}), nullptr));
			}), nullptr));
	_node->addChild(cabbageExplode);
}

void Cabbage::setZombiePosition(const Vec2& position)
{
	_zombiePosition = position; /* 记录僵尸位置*/
}

bool Cabbage::getBulletIsSameLineWithZombie(Zombies* zombie)
{
	return (fabs(zombie->getZombieAnimation()->getPositionY() - _position.y) <= 10 &&
		fabs(zombie->getZombieAnimation()->getPositionY() - _bulletAnimation->getPositionY()) <= 170) ? true : false;
}

bool Cabbage::getBulletIsEncounterWithZombie(Zombies* zombie)
{
	return fabs(zombie->getZombieAnimation()->getPositionX() - _bulletAnimation->getPositionX()) <= 60 ? true : false;
}

void Cabbage::bulletAttackHurtZombies(Zombies* zombie)
{
	if (zombie->getZombieCurrentHeadShieldVolume() < _attack) /* 如果当前身体护盾加头部护盾血量小于爆炸伤害 */
	{
		if (zombie->getZombieCurrentHeadShieldVolume() + zombie->getZombieCurrentBloodVolume() <= _attack) /* 如果僵尸所有血量小于爆炸伤害（僵尸死亡） */
		{
			/* 僵尸死亡 */
			zombie->setZombieCurrentBloodVolume(0);
			zombie->setZombieCurrentHeadShieldVolume(0);
		}
		else
		{
			/* 计算僵尸本体血量 */
			zombie->setZombieCurrentBloodVolume(zombie->getZombieCurrentHeadShieldVolume() + zombie->getZombieCurrentBloodVolume() - _attack);
			zombie->setZombieCurrentHeadShieldVolume(0);
		}
	}
	else
	{
		/* 计算僵尸护盾剩于血量 */
		zombie->setZombieCurrentHeadShieldVolume(zombie->getZombieCurrentHeadShieldVolume() - _attack);
	}
}
