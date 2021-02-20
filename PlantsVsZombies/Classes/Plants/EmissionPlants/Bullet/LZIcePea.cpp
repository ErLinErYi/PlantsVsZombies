/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.17
 *Email: 2117610943@qq.com
 */

#include "LZIcePea.h"

#include "Based/LZPlayMusic.h"
#include "Zombies/LZZombies.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"

IcePea::IcePea(Node* node)
{
    _node = node;
    _attack = 20;
    _bulletType = BulletType::IcePea;

    _peaAnimationName = "ice";

    srand(time(nullptr));
}

IcePea::~IcePea()
{
}

void IcePea::createShadow()
{ 
    /* 创建影子 */
    auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
    shadow->setName("plantshadow");
    shadow->setPosition(Vec2(0, -80));
    shadow->setOpacity(200);
    shadow->setLocalZOrder(_bulletAnimation->getLocalZOrder());
    _bulletAnimation->addChild(shadow, -1);
}

void IcePea::bulletAndZombiesCollision()
{
    for (auto zombie : ZombiesGroup)
    {
        if (!_isUsed && getBulletIsSameLineWithZombie(zombie) &&             /* 豌豆没有被使用 && 豌豆与僵尸在同一行 */
            zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() && /* 僵尸没有死亡 && 僵尸进入地图内 */
            getBulletIsEncounterWithZombie(zombie))                          /* 豌豆与僵尸碰撞 */
        {
            selectSoundEffect(zombie->getZombieBodyAttackSoundEffect(),
                zombie->getZombieHeadAttackSoundEffect());  /* 播放指定音乐 */

            setBulletOpacity();                /* 子弹消失 */
            attackZombies(zombie);             /* 攻击僵尸 */
            setBulletAttack(0);
            setBulletIsUsed(true);

            break; /* 一个豌豆只能击中一个僵尸 */
        }
    }
}

void IcePea::attackZombies(Zombies* zombie)
{
    icePeaExplodeEffect(zombie);       /* 寒冰豌豆使僵尸减速 */
    bulletAttackHurtZombies(zombie);   /* 僵尸减少生命值 */
    createPeaExplode();
}

void IcePea::createPeaExplode()
{
    static string animation[] = { {"IcePea_Explode_1"},{"IcePea_Explode_2"} };

    auto peaExplode = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("PeaExplode")->second);
    peaExplode->setPosition(getBulletPosition() + Vec2(20, 5));
    peaExplode->setAnimation(0, animation[rand() % 2], false);
    peaExplode->update(0);
    peaExplode->setScale(1.7f);
    peaExplode->setScaleX(-peaExplode->getScaleX());
    peaExplode->setLocalZOrder(_bulletAnimation->getLocalZOrder());
    _node->addChild(peaExplode);

    peaExplode->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([peaExplode]()
        {
            peaExplode->removeFromParent();
        }), nullptr));

    if (!(rand() % 3)) createExplodeEffect();
}

void IcePea::icePeaExplodeEffect(Zombies* zombie)
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

void IcePea::readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
    _bulletAnimation->setPosition(Vec2(
        (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionX"].GetFloat(),
        (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionY"].GetFloat()));
}
