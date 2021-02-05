/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.27
 *Email: 2117610943@qq.com
 */

#include "LZPEBStarFruitBullet.h"
#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"

StarFruitBullet::StarFruitBullet(Node* node):
	_direction(StarFruitBulletDirection::null)
,   _locationY(0)
,   _distance(69)
{
	_node = node;
	_attack = 20;
	_bulletType = BulletType::StarFruitBullet;
}

StarFruitBullet::~StarFruitBullet()
{
}

void StarFruitBullet::createBullet()
{
	bulletInit("StarFruitBullet", "animation");
	_bulletAnimation->setPosition(_position);
	_bulletAnimation->setTimeScale(1.5f);
	
	createShadow();
}

void StarFruitBullet::createShadow()
{
	/* 创建影子 */
	auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
	shadow->setName("shadow");
	shadow->setPosition(Vec2(0, -50));
	shadow->setOpacity(200);
	shadow->setScale(0.7f);
	shadow->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	_bulletAnimation->addChild(shadow, -1);
}

void StarFruitBullet::setBulletAction(StarFruitBulletDirection action)
{
	float actionX, actionY, time;
	_direction = action;
	switch (action)
	{
	case StarFruitBullet::StarFruitBulletDirection::Up:                time = 2.8f, actionX = 0,              actionY = 1000;      break;
	case StarFruitBullet::StarFruitBulletDirection::Down:              time = 2.8f, actionX = 0,              actionY = -1000;     break;
	case StarFruitBullet::StarFruitBulletDirection::Rear:              time = 2.8f, actionX = -1000,          actionY = 0;         break;
	case StarFruitBullet::StarFruitBulletDirection::DiagonallyAbove:   time = 4.0f, actionX = 1000 * sqrt(3), actionY = 1000;      break;
	case StarFruitBullet::StarFruitBulletDirection::ObliquelyBelow:    time = 4.0f, actionX = 1000 * sqrt(3), actionY = -1000;     break;
	}

	_bulletAnimation->runAction(Sequence::create(MoveBy::create(time, Vec2(actionX, actionY)),
		CallFunc::create([this]()
			{
				_bulletAnimation->setVisible(false);
			}), nullptr));

	_locationY = _bulletAnimation->getPositionY();
	_bulletAnimation->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.02f),
		CallFunc::create([=]()
			{
				if (_locationY - _bulletAnimation->getPositionY() > _distance) /* 第一次间隔69进行设置localZorder */
				{
					auto order = _bulletAnimation->getLocalZOrder();
					if (order > 0 && order < 100)
					{
						_bulletAnimation->setLocalZOrder(order + (_locationY > _bulletAnimation->getPositionY() ? 20 : -20));
					}
					_locationY = _bulletAnimation->getPositionY();

					_distance = 138; /* 第一次之后每隔138设置一次 */
				}
			}), nullptr)));
}

void StarFruitBullet::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&      /* 没有被使用 && 僵尸没有死亡 && 僵尸进入地图内 */
			getBulletIsEncounterWithZombie(zombie))                                           /* 子弹与僵尸碰撞 */
		{
			selectSoundEffect(
				zombie->getZombieBodyAttackSoundEffect(),
				zombie->getZombieHeadAttackSoundEffect());  /* 播放指定音乐 */

			attackZombies(zombie);               /* 攻击僵尸 */
			setBulletOpacity();                  /* 子弹消失 */
			createBulletExplode(zombie);         /* 创建爆炸动画 */
			setBulletAttack(0);                  /* 攻击力归零 */
			setBulletIsUsed(true);               /* 设置子弹被使用 */

			break; /* 一个子弹只能击中一个僵尸 */
		}
	}
}

bool StarFruitBullet::getBulletIsEncounterWithZombie(Zombies* zombie)
{
	auto &rect = zombie->getZombieAnimation()->getBoundingBox();
	return _bulletAnimation->getBoundingBox().intersectsRect(Rect(rect.origin.x + 60, rect.origin.y + 110, 60, 1));
}

void StarFruitBullet::bulletFadeOut()
{
	if (_bulletAnimation->getPositionY() > 810 || _bulletAnimation->getPositionY() < 110 ||
		_bulletAnimation->getPositionX() < 570)
	{
		if (_bulletAnimation->getOpacity() == 255)
		{
			auto action = Sequence::create(FadeOut::create(0.1f), nullptr);
			_bulletAnimation->getChildByName("shadow")->runAction(action);
			_bulletAnimation->runAction(action);
		}
	}
}

void StarFruitBullet::attackZombies(Zombies* zombie)
{
	bulletAttackHurtZombies(zombie);   /* 僵尸减少生命值 */
	zombie->setZombieHurtBlink();
}

void StarFruitBullet::createBulletExplode(Zombies* zombie)
{
	auto iter = _global->userInformation->getAnimationData().find("StarFruitBulletFracture");
	if (iter != _global->userInformation->getAnimationData().end())
	{
		auto explode = SkeletonAnimation::createWithData(iter->second);
		explode->setAnimation(0, "Fracture", false);
		explode->setPosition(zombie->getZombieAnimation()->getPosition() + Vec2(0, 80));
		explode->setLocalZOrder(100);
		explode->setScale(0.5f);
		explode->setScaleX(rand() % 2 ? -0.5 : 0.5);
		explode->update(0);
		_node->addChild(explode);

		explode->runAction(Sequence::create(DelayTime::create(1.1f), 
			CallFunc::create([explode]()
				{
					explode->removeFromParent();
				}), nullptr));
	}
}

void StarFruitBullet::caveBulletInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator)
{
	object.AddMember("Direction", static_cast<unsigned int>(_direction), allocator);
}

void StarFruitBullet::readBulletInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_direction = static_cast<StarFruitBulletDirection>((*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["Direction"].GetInt());
}

void StarFruitBullet::readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_bulletAnimation->setPosition(Vec2(
		(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionX"].GetFloat(),
		(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionY"].GetFloat()));
	setBulletAction(_direction);
}
