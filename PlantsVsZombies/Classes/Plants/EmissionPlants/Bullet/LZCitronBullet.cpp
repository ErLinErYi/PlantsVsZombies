/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.27
 *Email: 2117610943@qq.com
 */

#include "LZCitronBullet.h"

#include "../LZPeaShooter.h"
#include "../../LZPlants.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSBackgroundLayer.h"
#include "Based/LZPlayMusic.h"

CitronBullet::CitronBullet(Node* node, int id):
	_animationId(id)
{
	_node = node;
	_attack = 400;
	_bulletType = BulletType::CitronBullet;
}

CitronBullet::~CitronBullet()
{
}

void CitronBullet::createBullet()
{
    bulletInit("CitronBullet", _animationId ? "animation1" : "animation");

	_bulletAnimation->setPosition(_position + Vec2(0, 70));
	_bulletAnimation->setScale(1.7f);
	_bulletAnimation->runAction(Sequence::create(MoveBy::create(16.0f, Vec2(8000, 0)),
		CallFunc::create([this]()
			{
				_bulletAnimation->setVisible(false);
			}), nullptr));

    createShadow();
}

void CitronBullet::setAnimationId(const int tag)
{
	_animationId = tag;
}

int CitronBullet::getAnimationId()
{
	return _animationId;
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

void CitronBullet::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && getBulletIsSameLineWithZombie(zombie) &&             /* 没有被使用 && 僵尸在同一行 */
			zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() && /* 僵尸没有死亡 && 僵尸进入地图内 */
			getBulletIsEncounterWithZombie(zombie))                          /* 与僵尸碰撞 */
		{
			if (_animationId)
			{
				if (zombie->getZombieType() != ZombiesType::GargantuarZombies)
				{
					if (!zombie->getZombieIsStrikeFly())
					{
						PlayMusic::playMusic("ignite");
						zombie->getZombieAnimation()->getChildByName("shadow")->setVisible(false);
						zombie->getZombieAnimation()->stopAllActions();
						zombie->getZombieAnimation()->setAnimation(0, "Zombies_Stand", false);
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
					attackZombies(zombie);        /* 爆炸对僵尸造成伤害 */
				}
			}
			else
			{
				PlayMusic::playMusic("cherrybomb");

				attackZombies(zombie);        /* 爆炸对僵尸造成伤害 */
				splashDamageZombies(zombie);  /* 对僵尸造成溅伤*/
				setBulletOpacity();           /* 子弹消失 */
				createExplode();              /* 创建爆炸动画 */
				setBulletAttack(0);
				setBulletIsUsed(true);

				break;
			}
		}
	}
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

void CitronBullet::attackZombies(Zombies* zombie)
{
	bulletAttackHurtZombies(zombie);
	zombie->setZombieHurtBlink();
}

void CitronBullet::splashDamageZombies(Zombies* exceptZombie)
{
	/* 计算溅射伤害僵尸数 */
	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsEnterMap() && zombie->getZombieIsSurvive() && getZombieInExplodeRange(zombie))
		{
			++_zombieInExplodeRangeNumbers;
		}
	}

	for (auto zombie : ZombiesGroup)
	{
		if (exceptZombie != zombie && zombie->getZombieIsEnterMap() &&
			zombie->getZombieIsSurvive() && getZombieInExplodeRange(zombie))
		{
			/* 溅射伤害计算 */
			if (int(_attack / 3) * _zombieInExplodeRangeNumbers > _attack * 7)
			{
				_attack = max(int(7 * pow(_attack, 2) / (int(_attack / 3) * 3 * _zombieInExplodeRangeNumbers)), 1);
			}
			else
			{
				_attack = int(_attack / 3);
			}

			attackZombies(zombie);
		}
	}

	_zombieInExplodeRangeNumbers = 0;
}

bool CitronBullet::getZombieInExplodeRange(Zombies* zombie)
{
	/* 僵尸是否在爆炸范围判断 */
	return sqrt(pow(zombie->getZombieAnimation()->getPositionX() - _bulletAnimation->getPositionX(), 2) +
		pow((zombie->getZombieAnimation()->getPositionY() + 50) - (_bulletAnimation->getPositionY() - 40), 2)) <= 200 ? true : false;
}

void CitronBullet::caveBulletInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator)
{
	object.AddMember("AnimationId", _animationId, allocator);
}

void CitronBullet::readBulletInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_animationId = (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["AnimationId"].GetInt();
}

void CitronBullet::readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_bulletAnimation->setPosition(Vec2(
		(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionX"].GetFloat(),
		(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionY"].GetFloat()));
}
