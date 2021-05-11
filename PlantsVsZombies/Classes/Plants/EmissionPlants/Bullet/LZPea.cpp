/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.18
 *Email: 2117610943@qq.com
 */

#include "LZPea.h"
#include "../LZPeaShooter.h"

#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"

Pea::Pea(Node* node) :
    _torchwoodTag(0)
,   _actionTime(0.3f)
,   _initPostion{ {200,120},{200,-120} }
,   _peaDirectionType(PeaDirectionType::Normal)
,   _peaAnimationName("normal")
{
	_node = node;
	_attack = 20;
	_bulletType = BulletType::Pea;

	srand(time(nullptr));
}

Pea::~Pea()
{
}

void Pea::createBullet()
{
	bulletInit("FirePea", _peaAnimationName);
	_bulletAnimation->setPosition(_position + Vec2(70, 85));

	setBulletAction();
}

void Pea::setBulletAction()
{
	FiniteTimeAction* action;
	switch (_peaDirectionType)
	{
	case Pea::PeaDirectionType::Up:
		action = Sequence::create(MoveBy::create(_actionTime, _initPostion[0]), MoveBy::create(10.f, Vec2(7000, rand() % 21 - 10)), nullptr);
		createShadow(Vec2(0, 10));
		break;
	case Pea::PeaDirectionType::Normal:
		action = MoveBy::create(10.0f, Vec2(7000, rand() % 41 - 20));
		createShadow();
		break;
	case Pea::PeaDirectionType::Down:
		action = Sequence::create(MoveBy::create(_actionTime, _initPostion[1]), MoveBy::create(10.f, Vec2(7000, rand() % 21 - 10)), nullptr);
		createShadow(Vec2(0, -10));
		break;
	}

	_bulletAnimation->runAction(Sequence::create(action,
		CallFunc::create([this]()
			{
				_bulletAnimation->setVisible(false);
			}), nullptr));
}

void Pea::createShadow(Vec2 position)
{
	/* 创建影子 */
	auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
	shadow->setName("shadow");
	shadow->setPosition(Vec2(0, -80) + position);
	shadow->setScale(0.8f);
	shadow->setOpacity(200);
	shadow->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	_bulletAnimation->addChild(shadow, -1);
}

void Pea::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && getBulletIsSameLineWithZombie(zombie) &&                          /* 豌豆没有被使用 && 豌豆与僵尸在同一行 */
			zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&              /* 僵尸没有死亡 && 僵尸进入地图内 */
			getBulletIsEncounterWithZombie(zombie) && zombie->getZombieIsCanBeAttack())   /* 与僵尸碰撞 && 僵尸可以被攻击到 */
		{
			selectSoundEffect(zombie->getZombieBodyAttackSoundEffect(),
				zombie->getZombieHeadAttackSoundEffect());  /* 播放指定音乐 */

			attackZombies(zombie);
			setBulletOpacity();               /* 子弹消失 */
			createPeaExplode();               /* 创建豌豆爆炸动画 */
			setBulletAttack(0);
			setBulletIsUsed(true);

			break; /* 一个豌豆只能击中一个僵尸 */
		}
	}
}

void Pea::attackZombies(Zombies* zombie)
{
	bulletAttackHurtZombies(zombie);   /* 僵尸减少生命值 */
	zombie->setZombieHurtBlink();
}

bool Pea::getZombieInExplodeRange(Zombies* zombie)
{
	/* 僵尸是否在溅射范围判断 */
	return sqrt(pow(zombie->getZombieAnimation()->getPositionX() - _bulletAnimation->getPositionX(), 2) +
		pow((zombie->getZombieAnimation()->getPositionY() + 50) - (_bulletAnimation->getPositionY() - 25), 2)) <= 130 ? true : false;
}

void Pea::createPeaExplode()
{
	static string Animation[] = { {"Pea_Explode_1"},{"Pea_Explode_2"},{"Pea_Explode_3"},{"Pea_Explode_4"}};
	
	auto peaExplode = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("PeaExplode")->second);
	peaExplode->setPosition(getBulletPosition() - Vec2(35, 0));
	peaExplode->setAnimation(0, Animation[rand() % 4], false);
	peaExplode->update(0);
	peaExplode->setScale(0.9f);
	peaExplode->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	_node->addChild(peaExplode);

	peaExplode->runAction(Sequence::create(DelayTime::create(0.8f),
		CallFunc::create([peaExplode]()
		{
			peaExplode->removeFromParent();
		}), nullptr));

	if (!(rand() % 3)) createExplodeEffect();
}

void Pea::createExplodeEffect()
{
	auto effect = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("ExplodeEffect")->second);
	effect->setAnimation(0, "effect", false);
	effect->setPosition(getBulletPosition());
	effect->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	effect->setScale(2.0f);
	effect->update(0);
	effect->setOpacity(200);
	_node->addChild(effect);

	effect->runAction(Sequence::create(DelayTime::create(0.8f),
		CallFunc::create([effect]()
			{
				effect->removeFromParent();
			}), nullptr));
}

void Pea::setPeaDirectionType(PeaDirectionType type)
{
	_peaDirectionType = type;
}

Pea::PeaDirectionType Pea::getPeaDirectionType()
{
	return _peaDirectionType;
}

int Pea::getTorchwoodTag() const
{
	return _torchwoodTag;
}

void Pea::setTorchwoodTag(const int torchwoodTag)
{
	_torchwoodTag = torchwoodTag;
}

void Pea::calculateDirectionDistance(Vec2 initPosition, Vec2 position)
{
	if (_peaDirectionType != PeaDirectionType::Normal)
	{
		_actionTime = fabs((120 - fabs(position.y - initPosition.y))) / 400;

		if (120 - fabs(position.y - initPosition.y) > 10 && position.x - initPosition.x < 200)
		{
			if (_peaDirectionType == PeaDirectionType::Up)
			{
				_initPostion[0].y = 120 - fabs(position.y - initPosition.y);
				_initPostion[0].x = 200 - fabs(position.x - initPosition.x);
			}
			else
			{
				_initPostion[1].y = -(120 - fabs(position.y - initPosition.y));
				_initPostion[1].x = 200 - fabs(position.x - initPosition.x);
			}
		}
		else
		{
			_actionTime = 0;
			_initPostion[0] = { 0,0 };
			_initPostion[1] = { 0,0 };
		}
	}
}

void Pea::caveBulletInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator)
{
	object.AddMember("PeaDirectionType", static_cast<unsigned int>(_peaDirectionType), allocator);

	if (_peaDirectionType != PeaDirectionType::Normal)
	{
		object.AddMember("PeaInitPositionX", _position.x + 70, allocator);
		object.AddMember("PeaInitPositionY", _position.y + 85, allocator);
	}
}

void Pea::readBulletInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_peaDirectionType= static_cast<PeaDirectionType>((*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PeaDirectionType"].GetInt());

	_position = {
		(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionX"].GetFloat(),
		(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionY"].GetFloat() };

	// 上下方向运动时间位置计算
	if (_peaDirectionType != PeaDirectionType::Normal)
	{
		Vec2 initPosition{
			(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PeaInitPositionX"].GetFloat(),
			(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PeaInitPositionY"].GetFloat() };

		calculateDirectionDistance(initPosition, _position);
	}
}

void Pea::readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_bulletAnimation->setPosition(_position);
}
