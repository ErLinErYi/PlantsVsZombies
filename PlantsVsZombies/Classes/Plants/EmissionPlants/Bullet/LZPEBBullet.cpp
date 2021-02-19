/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.11
 *Email: 2117610943@qq.com
 */

#include "LZPEBBullet.h"

#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"
#include "Scenes/GameScene/LZSGControlLayer.h"
#include "Based/LZBPlayMusic.h"

Bullet::Bullet() :
	_node(nullptr)
,   _attack(0)
,   _zombieInExplodeRangeNumbers(0)
,   _bulletRow(-1)
,   _isUsed(false)
,   _bulletType(BulletType::None)
,   _position(Vec2::ZERO)
,   _global(Global::getInstance())
{
	srand(time(nullptr));
}

Bullet::~Bullet()
{
}

void Bullet::bulletDeleteUpdate(list<Bullet*>::iterator& bullet)
{
	if (!(*bullet)->getBulletVisible())
	{
		(*bullet)->releaseBullet();
		delete *bullet;
		*bullet = nullptr;
		BulletGroup.erase(bullet++);
	}
	else
	{
		++bullet;
	}
}

SkeletonAnimation* Bullet::bulletInit(const std::string& plantname, const std::string& animaionname)
{
	auto iter = _global->userInformation->getAnimationData().find(plantname);
	if (iter != _global->userInformation->getAnimationData().end())/* 如果可以找到 */
	{
		_bulletAnimation = SkeletonAnimation::createWithData(iter->second);
		_bulletAnimation->setAnimation(0, animaionname, true);
		_bulletAnimation->setLocalZOrder(getZOrder());
		_bulletAnimation->update(0);
		_node->addChild(_bulletAnimation);
		return _bulletAnimation;
	}
	return nullptr;
}

SkeletonAnimation* Bullet::getBullet() const
{
	return _bulletAnimation;
}

void Bullet::setBulletPosition(const Vec2& position)
{
	_position = position;
}

void Bullet::setBulletInRow(const int row)
{
	_bulletRow = row;
}

void Bullet::setBulletName(const string& name)
{
	_bulletName = name;
}

void Bullet::setBulletIsUsed(const bool isUsed)
{
	_isUsed = isUsed;
}

void Bullet::setBulletAttack(const float attack)
{
	_attack = attack;
}

void Bullet::setBulletOpacity(GLubyte opacity)
{
	_bulletAnimation->setOpacity(opacity);
	_bulletAnimation->getChildByName("shadow")->setOpacity(opacity);
}

void Bullet::setBulletOpacity()
{
	_bulletAnimation->setOpacity(0);
	_bulletAnimation->getChildByName("shadow")->setOpacity(0);
}

bool Bullet::getBulletIsUsed() const
{
	return _isUsed;
}

int Bullet::getBulletAttack() const
{
	return _attack;
}

void Bullet::bulletAttackHurtZombies(Zombies* zombie)
{
	if (zombie->getZombieCurrentBodyShieldVolume() < _attack)     /* 如果当前身体护盾血量小于爆炸伤害 */
	{
		if (zombie->getZombieCurrentBodyShieldVolume() +
			zombie->getZombieCurrentHeadShieldVolume() < _attack) /* 如果当前身体护盾加头部护盾血量小于爆炸伤害 */
		{
			if (zombie->getZombieCurrentBodyShieldVolume() + 
				zombie->getZombieCurrentHeadShieldVolume() + 
				zombie->getZombieCurrentBloodVolume() <= _attack) /* 如果僵尸所有血量小于爆炸伤害（僵尸死亡） */
			{
				zombie->setZombieDeath(true);
			}
			else
			{
				/* 计算僵尸本体血量 */
				zombie->setZombieCurrentBloodVolume(
					zombie->getZombieCurrentBodyShieldVolume() + 
					zombie->getZombieCurrentHeadShieldVolume() + 
					zombie->getZombieCurrentBloodVolume() - _attack);
				zombie->setZombieCurrentHeadShieldVolume(0);
				zombie->setZombieCurrentBodyShieldVolume(0);
			}
		}
		else
		{
			/* 计算僵尸护盾剩于血量 */
			zombie->setZombieCurrentHeadShieldVolume(
				zombie->getZombieCurrentBodyShieldVolume() + 
				zombie->getZombieCurrentHeadShieldVolume() - _attack);
			zombie->setZombieCurrentBodyShieldVolume(0);
		}
	}
	else
	{
		zombie->setZombieCurrentBodyShieldVolume(
			zombie->getZombieCurrentBodyShieldVolume() - _attack);
	}
}

void Bullet::bulletOutMapSetInvisible()
{
	auto pos = _bulletAnimation->getPosition();
	if (pos.x > controlLayerInformation->gameMapInformation->mapRight + 600 ||
		pos.y > controlLayerInformation->gameMapInformation->mapTop + 600 ||
		pos.x < 0 || pos.y < 0)
	{
		_bulletAnimation->setVisible(false);
	}
}

string& Bullet::getBulletName()
{
	return _bulletName;
}

float Bullet::getZOrder() const
{
	return (50 - _bulletRow) * 100 + 70;
}

void Bullet::releaseBullet()
{
	_bulletAnimation->removeFromParent();
	_bulletAnimation = nullptr;
}

Vec2 Bullet::getBulletPosition() const
{
	return _bulletAnimation->getPosition();
}

Vec2 Bullet::getBulletInitPosition() const
{
	return _position;
}

int Bullet::getBulletInRow() const
{
	return _bulletRow;
}

float Bullet::getBulletPositionX() const
{
	return _bulletAnimation->getPositionX();
}

float Bullet::getBulletPositionY() const
{
	return _bulletAnimation->getPositionY();
}

BulletType Bullet::getBulletType() const
{
	return _bulletType;
}

bool Bullet::getBulletIsSameLineWithZombie(Zombies* zombie)
{
	return zombie->getZombieInRow() == getBulletInRow() ? true : false;
}

bool Bullet::getBulletIsEncounterWithZombie(Zombies* zombie)
{
	return fabs(zombie->getZombieAnimation()->getPositionX() - _bulletAnimation->getPositionX()) <= 25 ? true : false;
}

bool Bullet::getBulletVisible() const
{
	return _bulletAnimation->isVisible();
}

void Bullet::selectSoundEffect(const int body, const int head)
{
	if (body)
	{
		playSoundEffect(SoundEffectType::shieldhit);
	}
	else
	{
		selectSoundEffect(head);
	}
}

void Bullet::selectSoundEffect(const int head)
{
	switch (head)
	{
	case 0: playSoundEffect(SoundEffectType::kernelpult); break;
	case 1: playSoundEffect(SoundEffectType::shieldhit);  break;
	case 2: playSoundEffect(SoundEffectType::plastichit); break;
	}
}

void Bullet::playSoundEffect(SoundEffectType soundEffect)
{
	switch (soundEffect)
	{
	case SoundEffectType::kernelpult:  PlayMusic::playMusic(rand() % 2 ? "kernelpult" : "kernelpult2");   break;
	case SoundEffectType::shieldhit:   PlayMusic::playMusic(rand() % 2 ? "shieldhit" : "shieldhit2");     break;
	case SoundEffectType::plastichit:  PlayMusic::playMusic(rand() % 2 ? "plastichit" : "plastichit2");   break;
	case SoundEffectType::firepea:     PlayMusic::playMusic(rand() % 2 ? "ignite" : "ignite2");           break;
	case SoundEffectType::watermelon:  PlayMusic::playMusic(rand() % 2 ? "melonimpact" : "melonimpact2"); break;
	}
}