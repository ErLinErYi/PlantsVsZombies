/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.04
 *Email: 2117610943@qq.com
 */

#include "LZWaterMelonBullet.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"

WaterMelonBullet::WaterMelonBullet(Node* node)
{
	_node = node;
	_attack = 80;
	_bulletType = BulletType::WaterMelonBullet;
}

WaterMelonBullet::~WaterMelonBullet()
{
}

void WaterMelonBullet::createBullet()
{
	calculateInformationForReadFile();

	bulletInit("WaterMelon", "WaterMelon_Normal");

	_bulletAnimation->setScale(0.8f * _scale);
	_bulletAnimation->setPosition(_initPosition);

	createShadow(1.5f);

	createListener("WaterMelon_Explode");
}

void WaterMelonBullet::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && zombie->getZombieIsSurvive() &&                                           /* 没有使用 && 僵尸没有死亡 */
			getBulletIsSameLineWithZombie(zombie) && getBulletIsEncounterWithZombie(zombie))      /* 子弹与僵尸同一行 && 子弹与僵尸碰撞 */
		{
			playSoundEffect(SoundEffectType::watermelon);

			bulletAttackHurtZombies(zombie);   /* 僵尸减少生命值 */
			_bulletAnimation->setOpacity(0);
			
			createExplodeAnimation("WaterMelon","WaterMelon_Explode");

			setBulletIsUsed(true);
			break; /* 一个只能击中一个僵尸 */
		}
	}
}

void WaterMelonBullet::bulletAttackHurtZombies(Zombies* zombie)
{
	Cabbage::bulletAttackHurtZombies(zombie);
	zombie->setZombieHurtBlink();

	splashDamageZombies(zombie);  /* 对僵尸造成溅伤*/
}

void WaterMelonBullet::splashDamageZombies(Zombies* exceptZombie)
{
	/* 计算溅射伤害僵尸数 */
	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsEnterMap() && zombie->getZombieIsSurvive() && getZombieInExplodeRange(zombie, exceptZombie))
		{
			++_zombieInExplodeRangeNumbers;
		}
	}

	for (auto zombie : ZombiesGroup)
	{
		if (exceptZombie != zombie && zombie->getZombieIsEnterMap() &&
			zombie->getZombieIsSurvive() && getZombieInExplodeRange(zombie, exceptZombie))
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

			Cabbage::bulletAttackHurtZombies(zombie);
			zombie->setZombieHurtBlink();
		}
	}

	_zombieInExplodeRangeNumbers = 0;
}

bool WaterMelonBullet::getZombieInExplodeRange(Zombies* zombie, Zombies* exceptZombie)
{
	const auto a = zombie->getZombieAnimation()->getPositionX();
	const auto b = exceptZombie->getZombieAnimation()->getPositionX();
	const auto c = zombie->getZombieInRow();
	const auto d = exceptZombie->getZombieInRow();
	
	if (c == d)
	{
		if ((a > b && (a - b) < 180) || (b > a && (b - a) < 90))return true;
	}
	else if (fabs(c - d) <= 1)
	{
		if ((a > b && (a - b) < 125) || (b > a && (b - a) < 70))return true;
	}
	return false;
}
