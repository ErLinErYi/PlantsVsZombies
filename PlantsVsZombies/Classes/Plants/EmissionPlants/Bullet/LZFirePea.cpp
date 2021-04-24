/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.19
 *Email: 2117610943@qq.com
 */

#include "LZFirePea.h"
#include "LZIcePea.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"

FirePea::FirePea(Node* node)
{
    _node = node;
    _attack = 40;
    _bulletType = BulletType::FirePea;

    _peaAnimationName = "fire";
}

FirePea::~FirePea()
{
}

void FirePea::createBullet()
{
    bulletInit("FirePea", _peaAnimationName);

    _bulletAnimation->setPosition(_position + Vec2(70, 85));
    _bulletAnimation->setScale(1.5f);

    setBulletAction();
}

void FirePea::createShadow(Vec2 position)
{
    /* 创建影子 */
    auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
    shadow->setScaleY(0.7f);
    shadow->setName("shadow");
    shadow->setOpacity(200);
    shadow->setPosition(Vec2(0, -52) + position);
    shadow->setLocalZOrder(_bulletAnimation->getLocalZOrder());
    _bulletAnimation->addChild(shadow, -1);
}

void FirePea::bulletAndZombiesCollision()
{
    for (auto zombie : ZombiesGroup)
    {
        if (!_isUsed && getBulletIsSameLineWithZombie(zombie) &&                          /* 豌豆没有被使用 && 豌豆与僵尸在同一行 */
            zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&              /* 僵尸没有死亡 && 僵尸进入地图内 */
            getBulletIsEncounterWithZombie(zombie) && zombie->getZombieIsCanBeAttack())   /* 与僵尸碰撞 && 僵尸可以被攻击到 */
        {
            playSoundEffect(SoundEffectType::firepea); /* 播放指定音乐 */

            attackZombies(zombie);                     /* 攻击伤害此僵尸 */
            splashDamageZombies(zombie);               /* 溅射伤害僵尸*/
            setBulletOpacity();                        /* 子弹消失 */
            createPeaExplode();                        /* 创建豌豆爆炸动画 */
            setBulletAttack(0);
            setBulletIsUsed(true);

            break;
        }
    }
}

void FirePea::attackZombies(Zombies* zombie)
{
    bulletAttackHurtZombies(zombie);
    recoveryFrozenZombie(zombie);
    zombie->setZombieHurtBlink();
}

void FirePea::splashDamageZombies(Zombies* exceptZombie)
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

void FirePea::createPeaExplode()
{
    auto peaExplode = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("PeaExplode")->second);
    peaExplode->setPosition(getBulletPosition());
    peaExplode->setAnimation(0, "FirePea_Explode", false);
    peaExplode->update(0);
    peaExplode->setScale(0.7f);
    peaExplode->setLocalZOrder(_bulletAnimation->getLocalZOrder());
    _node->addChild(peaExplode);

    peaExplode->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([peaExplode]()
        {
            peaExplode->removeFromParent();
        }), nullptr));
}

void FirePea::recoveryFrozenZombie(Zombies* zombie)
{
    if (zombie->getZombieTimerTime() != -1 || zombie->getZombieTimerTime(true) != -1)
    {
        zombie->setZombieActionRecovery();
    }
}

void FirePea::readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
    _bulletAnimation->setPosition(Vec2(
        (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionX"].GetFloat(),
        (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionY"].GetFloat()));
}