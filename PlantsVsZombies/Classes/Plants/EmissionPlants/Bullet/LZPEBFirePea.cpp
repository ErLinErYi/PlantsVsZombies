/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.19
 *Email: 2117610943@qq.com
 */

#include "LZPEBFirePea.h"
#include "LZPEBIcePea.h"
#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"

FirePea::FirePea(Node* node)
{
    _node = node;
    _attack = 40;
    _bulletType = BulletType::FirePea;

    _isFire = true;
    _fireNumbers = 1;
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
    _bulletAnimation->runAction(Sequence::create(MoveBy::create(3.0f, Vec2(2000, rand() % 31 - 15)),
        CallFunc::create([=]()
            {
                _bulletAnimation->setVisible(false);
            }), nullptr));

    createShadow();
}

void FirePea::createShadow()
{
    /* 创建影子 */
    auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
    shadow->setScaleY(0.7f);
    shadow->setName("shadow");
    shadow->setOpacity(200);
    shadow->setPosition(Vec2(0, -52));
    shadow->setLocalZOrder(_bulletAnimation->getLocalZOrder());
    _bulletAnimation->addChild(shadow, -1);
}

void FirePea::bulletAndZombiesCollision()
{
    for (auto zombie : ZombiesGroup)
    {
        if (!_isUsed && getBulletIsSameLineWithZombie(zombie) &&             /* 豌豆没有被使用 && 豌豆与僵尸在同一行 */
            zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() && /* 僵尸没有死亡 && 僵尸进入地图内 */
            getBulletIsEncounterWithZombie(zombie))                          /* 豌豆与僵尸碰撞 */
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

void FirePea::recoveryFrozenZombie(Zombies* zombie)
{
    if (zombie->getZombieTimerTime() != -1)
    {
        zombie->setZombieTimerTime(-1); /* 僵尸被寒冰射手减速时间清零 */
        IcePea::setZombieActionRecovery(zombie);
    }
}

void FirePea::readBulletInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
    _isFire = (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["IsFire"].GetBool();
    _fireNumbers = (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PeaFireNumbers"].GetInt();
}

void FirePea::readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
    _bulletAnimation->setPosition(Vec2(
        (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionX"].GetFloat(),
        (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionY"].GetFloat()));
    Bullet::setBulletOpacity((*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["Opacity"].GetInt());
}