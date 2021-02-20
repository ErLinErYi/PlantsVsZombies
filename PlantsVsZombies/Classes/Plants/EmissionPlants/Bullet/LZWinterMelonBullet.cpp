/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.04
 *Email: 2117610943@qq.com
 */

#include "LZWinterMelonBullet.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Based/LZPlayMusic.h"

WinterMelonBullet::WinterMelonBullet(Node* node)
{
	_node = node;
	_attack = 80;
	_bulletType = BulletType::WinterMelonBullet;
}

WinterMelonBullet::~WinterMelonBullet()
{
}

void WinterMelonBullet::createBullet()
{
	calculateInformationForReadFile();

	bulletInit("WinterMelon", "WinterMelon_Normal");

	_bulletAnimation->setScale(0.8f);
	_bulletAnimation->setPosition(_initPosition);

	createShadow(1.5f);

	createListener("WinterMelon_Explode", 1.5f);
}

void WinterMelonBullet::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && zombie->getZombieIsSurvive() &&                                           /* 没有使用 && 僵尸没有死亡 */
			getBulletIsSameLineWithZombie(zombie) && getBulletIsEncounterWithZombie(zombie))      /* 子弹与僵尸同一行 && 子弹与僵尸碰撞 */
		{
			playSoundEffect(SoundEffectType::watermelon);

			bulletAttackHurtZombies(zombie);   /* 僵尸减少生命值 */
			_bulletAnimation->setOpacity(0);
			
			createExplodeAnimation("WinterMelon", "WinterMelon_Explode", 1.5f);

			setBulletIsUsed(true);
			break; /* 一个只能击中一个僵尸 */
		}
	}
}

void WinterMelonBullet::bulletAttackHurtZombies(Zombies* zombie)
{
	Cabbage::bulletAttackHurtZombies(zombie);
	winterMelonExplodeEffect(zombie);
	splashDamageZombies(zombie);  /* 对僵尸造成溅伤*/
}

void WinterMelonBullet::winterMelonExplodeEffect(Zombies* zombie)
{
	zombie->setZombieTimerTime(10, true);

	if (!zombie->getZombieIsFrozen())//判断是否已经被减速
	{
		PlayMusic::playMusic("frozen");
		zombie->setZombieActionSlow();
		zombie->createZombieTimer();
	}
	else
	{
		zombie->setZombieHurtBlink();
	}
}
