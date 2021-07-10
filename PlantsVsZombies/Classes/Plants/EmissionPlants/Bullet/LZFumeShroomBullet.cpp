/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.09
 *Email: 2117610943@qq.com
 */

#include "LZFumeShroomBullet.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Based/LZPlayMusic.h"

FumeShroomBullet::FumeShroomBullet(Node* node)
{
	_node = node;
	_attack = 20;
	_animationLoop = false;
	_bulletType = BulletType::FumeShroomBullet;
}

FumeShroomBullet::~FumeShroomBullet()
{
}

void FumeShroomBullet::createBullet()
{
	bulletInit("FumeshroomBubbles", "animation");

	_bulletAnimation->setPosition(_position + Vec2(60, 70));
	_bulletAnimation->runAction(Sequence::create(DelayTime::create(2.f), 
		CallFunc::create([=]() 
			{
				_bulletAnimation->setVisible(false);
			}), nullptr));

	PlayMusic::playMusic("fume");
}

void FumeShroomBullet::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && getBulletIsSameLineWithZombie(zombie) &&                          /* 豌豆没有被使用 && 豌豆与僵尸在同一行 */
			zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&              /* 僵尸没有死亡 && 僵尸进入地图内 */
			getBulletIsEncounterWithZombie(zombie) && zombie->getZombieIsCanBeAttack())   /* 与僵尸碰撞 && 僵尸可以被攻击到 */
		{
			selectSoundEffect(zombie->getZombieBodyAttackSoundEffect(),
				zombie->getZombieHeadAttackSoundEffect());  /* 播放指定音乐 */

			bulletAttackHurtZombies(zombie);   /* 僵尸减少生命值 */
			zombie->setZombieHurtBlink();
		}
	}

	setBulletAttack(0);
	setBulletIsUsed(true);
}

bool FumeShroomBullet::getBulletIsEncounterWithZombie(Zombies* zombie)
{
	return  (zombie->getZombiePositionX() >= _position.x) && (zombie->getZombiePositionX() - _position.x < 600);
}