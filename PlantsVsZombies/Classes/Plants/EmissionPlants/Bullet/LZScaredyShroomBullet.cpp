/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.10
 *Email: 2117610943@qq.com
 */

#include "LZScaredyShroomBullet.h"

#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"

ScaredyShroomBullet::ScaredyShroomBullet(Node* node)
{
	_node = node;
	_attack = 20;
	_bulletType = BulletType::ScaredyShroomBullet;
}

ScaredyShroomBullet::~ScaredyShroomBullet()
{
}

void ScaredyShroomBullet::createBullet()
{
	bulletInit("BubblesBullet", "BubblesBullet_Normal");

	_bulletAnimation->setPosition(_position + Vec2(30, 50));
	_bulletAnimation->setScale(1.2f);
	_bulletAnimation->runAction(Sequence::create(MoveBy::create(16.0f, Vec2(8000, rand() % 31 - 15)),
		CallFunc::create([this]()
			{
				_bulletAnimation->setVisible(false);
			}), nullptr));

	createShadow();
}

void ScaredyShroomBullet::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && getBulletIsSameLineWithZombie(zombie) &&                         /* 没有被使用 && 与僵尸在同一行 */
			zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&             /* 僵尸没有死亡 && 僵尸进入地图内 */
			getBulletIsEncounterWithZombie(zombie) && zombie->getZombieIsCanBeAttack())  /* 柠檬酸与僵尸碰撞 && 僵尸可以被攻击到*/
		{
			selectSoundEffect(zombie->getZombieBodyAttackSoundEffect(), zombie->getZombieHeadAttackSoundEffect());  /* 播放指定音乐 */

			setBulletOpacity();                /* 子弹消失 */
			bulletAttackHurtZombies(zombie);   /* 僵尸减少生命值 */
			zombie->setZombieHurtBlink();
			createBulletExplode();   /* 创建爆炸动画 */
			setBulletAttack(0);
			setBulletIsUsed(true);

			break; /* 一个豌豆只能击中一个僵尸 */
		}
	}
}

bool ScaredyShroomBullet::getBulletIsEncounterWithZombie(Zombies* zombie)
{
	return fabs(zombie->getZombieAnimation()->getPositionX() - _bulletAnimation->getPositionX()) <= 25 ? true : false;
}

void ScaredyShroomBullet::createShadow()
{
	/* 创建影子 */
	auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
	shadow->setName("shadow");
	shadow->setPosition(Vec2(0, -40));
	shadow->setScale(0.5f);
	shadow->setOpacity(200);
	shadow->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	_bulletAnimation->addChild(shadow, -1);
}

void ScaredyShroomBullet::createBulletExplode()
{
	auto explode = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("BubblesBullet")->second);
	explode->setPosition(getBulletPosition());
	explode->setAnimation(0, rand() % 2 ? "BubblesBullet_Attack" : "BubblesBullet_Attack2", false);
	explode->setScale(1.2f);
	explode->update(0);
	explode->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	_node->addChild(explode);

	explode->runAction(Sequence::create(DelayTime::create(1.f), CallFunc::create([explode]()
		{
			explode->removeFromParent();
		}), nullptr));
}

void ScaredyShroomBullet::readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
	_bulletAnimation->setPosition(Vec2(
		(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionX"].GetFloat(),
		(*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionY"].GetFloat()));
}
