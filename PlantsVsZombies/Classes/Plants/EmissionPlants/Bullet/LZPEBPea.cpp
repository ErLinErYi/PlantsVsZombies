/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.18
 *Email: 2117610943@qq.com
 */

#include "LZPEBPea.h"
#include "../LZPEPeaShooter.h"

#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"

Pea::Pea(Node* node) :
	_isFire(false)
,   _isIce(false)
,   _torchwoodTag(0)
,   _fireNumbers(0)
,   _peaAnimationName("normal")
{
	_node = node;
	_attack = 20;
	_bulletType = BulletType::Pea;
}

Pea::~Pea()
{
}

void Pea::createBullet()
{
	bulletInit("FirePea", _peaAnimationName);
	_bulletAnimation->setPosition(_position + Vec2(70, 85));
	_bulletAnimation->runAction(Sequence::create(MoveBy::create(3.0f, Vec2(2000, rand() % 41 - 20)),
		CallFunc::create([this]()
			{
				_bulletAnimation->setVisible(false);
			}), nullptr));

	createShadow();
}

void Pea::createShadow()
{
	/* 创建影子 */
	auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
	shadow->setName("shadow");
	shadow->setPosition(Vec2(0, -80));
	shadow->setOpacity(200);
	shadow->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	_bulletAnimation->addChild(shadow, -1);
}

void Pea::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && getBulletIsSameLineWithZombie(zombie) &&              /* 豌豆没有被使用 && 豌豆与僵尸在同一行 */
			zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&  /* 僵尸没有死亡 && 僵尸进入地图内 */
			getBulletIsEncounterWithZombie(zombie))                           /* 豌豆与僵尸碰撞 */
		{
			selectSoundEffect(zombie->getZombieBodyAttackSoundEffect(),
				zombie->getZombieHeadAttackSoundEffect());  /* 播放指定音乐 */

			judgeAttackMethod(zombie);
			setBulletOpacity();               /* 子弹消失 */
			createPeaExplode();               /* 创建豌豆爆炸动画 */
			setBulletAttack(0);
			setBulletIsUsed(true);

			break; /* 一个豌豆只能击中一个僵尸 */
		}
	}
}

void Pea::judgeAttackMethod(Zombies* zombie)
{
	attackZombies(zombie);

	if (_isFire)
	{
		splashDamageZombies(zombie);
	}
}

void Pea::attackZombies(Zombies* zombie)
{
	bulletAttackHurtZombies(zombie);   /* 僵尸减少生命值 */
	zombie->setZombieHurtBlink();
}

void Pea::splashDamageZombies(Zombies* exceptZombie)
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
			if (int(_attack / 3) * _zombieInExplodeRangeNumbers > _attack)
			{
				_attack = max(int(pow(_attack, 2) / (int(_attack / 3) * 3 * _zombieInExplodeRangeNumbers)), 1);
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

bool Pea::getZombieInExplodeRange(Zombies* zombie)
{
	/* 僵尸是否在溅射范围判断 */
	return sqrt(pow(zombie->getZombieAnimation()->getPositionX() - _bulletAnimation->getPositionX(), 2) +
		pow((zombie->getZombieAnimation()->getPositionY() + 50) - (_bulletAnimation->getPositionY() - 25), 2)) <= 130 ? true : false;
}

void Pea::createPeaExplode()
{
	static string Animation[] = { {"Pea_Explode_1"},{"Pea_Explode_2"},{"Pea_Explode_3"},{"Pea_Explode_4"},{"FirePea_Explode"} };

	auto peaExplode = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("PeaExplode")->second);
	peaExplode->setPosition(_isFire ? getBulletPosition() : getBulletPosition() - Vec2(25, 0));
	peaExplode->setAnimation(0, Animation[_isFire ? 4 : rand() % 4], false);
	peaExplode->update(0);
	peaExplode->setScale(_isFire ? 1.4f : 0.9f);
	peaExplode->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	_node->addChild(peaExplode);

	peaExplode->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([peaExplode]()
		{
			peaExplode->removeFromParent();
		}), nullptr));
}

bool Pea::getIsFire() const
{
	return _isFire;
}

bool Pea::getIsIce() const
{
	return _isIce;
}

int Pea::getPeaFireNumbers() const
{
	return _fireNumbers;
}

int Pea::getTorchwoodTag() const
{
	return _torchwoodTag;
}

void Pea::setIsFire(const bool isFire)
{
	_isFire = isFire;
}

void Pea::setIsIce(const bool isIce)
{
	_isIce = isIce;
}

void Pea::setPeaFireNumbers(const int fireNumbers)
{
	_fireNumbers = fireNumbers;
}

void Pea::addPeaFireNumbers()
{
	++_fireNumbers;
}

void Pea::setTorchwoodTag(const int torchwoodTag)
{
	_torchwoodTag = torchwoodTag;
}

void Pea::caveBulletInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator)
{
	object.AddMember("IsFire", _isFire, allocator);
	object.AddMember("IsIce", _isIce, allocator);
	object.AddMember("PeaFireNumbers", _fireNumbers, allocator);
}

void Pea::readBulletInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_isFire = (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["IsFire"].GetBool();
	_fireNumbers = (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PeaFireNumbers"].GetInt();
}

void Pea::readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_bulletAnimation->setPosition(Vec2(
		(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionX"].GetFloat(),
		(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionY"].GetFloat()));
	Bullet::setBulletOpacity((*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["Opacity"].GetInt());

	if (_fireNumbers == 0)
	{
		_bulletAnimation->setAnimation(0, "normal", true);
	}
	if (_fireNumbers == 1)
	{
		_bulletAnimation->setAnimation(0, "fire", true);
		_bulletAnimation->getChildByName("shadow")->setPosition(Vec2(0, -52));
		_bulletAnimation->getChildByName("shadow")->setScaleY(0.7f);
		_bulletAnimation->getChildByName("shadow")->setScaleX(1.0f);
		_bulletAnimation->setScale(1.5f);
	}
}
