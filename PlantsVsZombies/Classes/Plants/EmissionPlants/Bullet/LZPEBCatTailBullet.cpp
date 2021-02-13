/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.7
 *Email: 2117610943@qq.com
 */
#include "LZPEBCatTailBullet.h"
#include "Zombies/LZZZombies.h"
#include "Scenes/GameScene/LZSGData.h"
#include <cocos/editor-support/spine/extension.h>

CatTailBullet::CatTailBullet(Node* node):
    _targetZombie(nullptr),
    _distance(0xffff),
    _speed{ 10,0 },
    _distanceY(69),
    _locationY(0),
    _isTrack(false)
{
    _node = node;
    _attack = 20;
    _bulletType = BulletType::CatTailBullet;
}

CatTailBullet::~CatTailBullet()
{
}

void CatTailBullet::createBullet()
{
    bulletInit("CatTailBullet", "CatTailBullet_Normal");
    _bulletAnimation->setPosition(_position + Vec2(70, 85));
    _bulletAnimation->setScale(0.6f);
    _bulletAnimation->setLocalZOrder(_bulletAnimation->getLocalZOrder() - 3);
    createShadow();
    createListener();
}

void CatTailBullet::createListener()
{
    _locationY = _bulletAnimation->getPositionY();
    _bulletAnimation->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.02f),
        CallFunc::create([=]()
            {
                if (fabs(_locationY - _bulletAnimation->getPositionY()) > _distanceY) /* 第一次间隔69进行设置localZorder */
                {
                    auto order = _bulletAnimation->getLocalZOrder();
                    if (order > 0 && order < 100)
                    {
                        _bulletAnimation->setLocalZOrder(order + (_locationY > _bulletAnimation->getPositionY() ? 20 : -20));
                    }
                    _locationY = _bulletAnimation->getPositionY();

                    _distanceY = 138; /* 第一次之后每隔138设置一次 */
                }
            }), nullptr)));
}

void CatTailBullet::createShadow()
{ 
    auto shadow = Sprite::createWithSpriteFrameName("plantshadow.png");
    shadow->setName("shadow");
    shadow->setOpacity(180);
    shadow->setScale(0.6f);
    shadow->setPosition(_bulletAnimation->getPosition() + Vec2(0, -120));
    shadow->setLocalZOrder(_bulletAnimation->getLocalZOrder());
    _node->addChild(shadow);
    shadow->runAction(RepeatForever::create(Sequence::create(
        CallFunc::create([=]()
            {
                shadow->setPosition(_bulletAnimation->getPosition() + Vec2(0, -120));
            }), DelayTime::create(0.01f),
                CallFunc::create([=]()
                    {
                        if (!_bulletAnimation->getOpacity())
                        {
                            shadow->removeFromParent();
                        }
                    }), nullptr)));
    shadow->runAction(Sequence::create(DelayTime::create(5.0f),
        CallFunc::create([shadow]()
            {
                shadow->removeFromParent();
            }), nullptr));
}

void CatTailBullet::bulletAndZombiesCollision()
{
    calculateBulletPosition();

    for (auto zombie : ZombiesGroup)
    {
        if (!_isUsed && zombie->getZombieIsSurvive() &&                              /* 没有被使用 &&  僵尸没有死亡 */
            zombie->getZombieIsEnterMap() && getBulletIsEncounterWithZombie(zombie)) /* 僵尸进入地图内 && 豌豆与僵尸碰撞 */
        {
            selectSoundEffect(zombie->getZombieBodyAttackSoundEffect(),
                zombie->getZombieHeadAttackSoundEffect());  /* 播放指定音乐 */

            bulletAttackHurtZombies(zombie);   /* 僵尸减少生命值 */
            zombie->setZombieHurtBlink();

            createBulletExplode(zombie);
            setBulletAttack(0);
            setBulletIsUsed(true);

            break; /* 一个豌豆只能击中一个僵尸 */
        }
    }
}

void CatTailBullet::calculateBulletPosition()
{
    do
    {
        if (_targetZombie)
        {
            if (!bulletIsInMap() || !_targetZombie->getZombieIsSurvive())
            {
                _targetZombie = nullptr;
                break;
            }

            auto rad = calculateAngle(_targetZombie->getZombieAnimation()->getPosition() + Vec2(0, 100));
            if (rad >= PI)
            {
                _targetZombie = nullptr;
                break;
            }

            auto deg = CC_RADIANS_TO_DEGREES(rad);
            auto deltaD = min(1.f, deg / _bulletAnimation->getPosition().distance(_targetZombie->getZombieAnimation()->getPosition()));
            auto deltaR = min(1.f, deltaD / PI);

            switch (calculateDirection(_targetZombie->getZombieAnimation()->getPosition() + Vec2(0, 100)))
            {
            case CatTailBullet::RatateDirtection::COUNTERCLOCKWISE:
                _speed = _speed.rotateByAngle(Vec2::ZERO, deltaR);
                _bulletAnimation->setRotation(_bulletAnimation->getRotation() - deltaD * 20);
                break;
            case CatTailBullet::RatateDirtection::CLOCKWISE:
                _speed = _speed.rotateByAngle(Vec2::ZERO, -deltaR);
                _bulletAnimation->setRotation(_bulletAnimation->getRotation() + deltaD * 20);
                break;
            default:break;
            }

            _isTrack = true;
        }
        else
        {
            if (bulletIsInMap() && !_isTrack)
            {
                seekZombie();
            }
        }
    } while (0);

    _bulletAnimation->setPosition(Vec2(_bulletAnimation->getPosition() + _speed));
}

bool CatTailBullet::getBulletIsEncounterWithZombie(Zombies* zombie)
{
    auto& rect = zombie->getZombieAnimation()->getBoundingBox();
    return _bulletAnimation->getBoundingBox().intersectsRect(Rect(rect.origin.x + 60, rect.origin.y + 110, 60, 1));
}

void CatTailBullet::seekZombie()
{
    for (auto zombie : ZombiesGroup)
    {
        if (!_isUsed && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap())
        {
            auto distance = calculateBulletAndZombie(zombie);
            if (distance < _distance)
            {
                _distance = distance;
                _targetZombie = zombie;
            }
        }
    }
}

float CatTailBullet::calculateBulletAndZombie(Zombies* zombie)
{
    return sqrt(pow(zombie->getZombieAnimation()->getPositionX() - _bulletAnimation->getPositionX(), 2.f) +
        pow(zombie->getZombieAnimation()->getPositionY() - _bulletAnimation->getPositionY(), 2.f));
}

float CatTailBullet::calculateAngle(Vec2 vec2)
{
    return Vec2::angle(_speed, vec2 - _bulletAnimation->getPosition());
}

CatTailBullet::RatateDirtection CatTailBullet::calculateDirection(Vec2 vec2)
{
    auto p = vec2 - _bulletAnimation->getPosition();
    if (_speed.cross(p) > 0)
    {
        return CatTailBullet::RatateDirtection::COUNTERCLOCKWISE;
    }
    else if(_speed.cross(p) <0)
    {
        return CatTailBullet::RatateDirtection::CLOCKWISE;
    }
    else
    {
        return CatTailBullet::RatateDirtection::NO_ROTATE;
    }
}

bool CatTailBullet::bulletIsInMap()
{
    return (_bulletAnimation->getPosition() < Vec2(1710, 970) && _bulletAnimation->getPosition() > Vec2(570, 110));
}

void CatTailBullet::createBulletExplode(Zombies* zombie)
{
    _bulletAnimation->setLocalZOrder(zombie->getZombieAnimation()->getLocalZOrder() + 1);
    _bulletAnimation->setOpacity(0);
    _bulletAnimation->runAction(Sequence::create(DelayTime::create(1.f), CallFunc::create([this]() {_bulletAnimation->setVisible(false); }), nullptr));

    auto explode= SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("CatTailBullet")->second);
	explode->setAnimation(0, "CatTailBullet_Explode", false);
    explode->setPosition(zombie->getZombieAnimation()->getPosition() + Vec2(0, 80));
	explode->setLocalZOrder(zombie->getZombieAnimation()->getLocalZOrder() + 1);
    explode->setScale(1.f + rand() % 5 / 10.f);
    explode->update(0);
    explode->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([explode]() {explode->removeFromParent(); }), nullptr));
    _node->addChild(explode);
}

void CatTailBullet::caveBulletInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator)
{
    object.AddMember("RotateTion", _bulletAnimation->getRotation(), allocator);
}

void CatTailBullet::readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i)
{
    _bulletAnimation->setPosition(Vec2(
        (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionX"].GetFloat(),
        (*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["PositionY"].GetFloat()));
    _bulletAnimation->setRotation((*levelDataDocument)[key]["Bullet"][to_string(i).c_str()]["RotateTion"].GetFloat());
}
