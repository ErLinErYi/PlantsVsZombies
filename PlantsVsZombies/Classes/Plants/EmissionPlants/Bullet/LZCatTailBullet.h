/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.7
 *Email: 2117610943@qq.com
 */
#pragma once
#include "LZBullet.h"
#include "Zombies/LZZombies.h"

class CatTailBullet :public Bullet
{
CC_CONSTRUCTOR_ACCESS:
    CatTailBullet(Node* node = nullptr);
    ~CatTailBullet();

protected:
    enum class RatateDirtection
    {
        COUNTERCLOCKWISE,
        CLOCKWISE,
        NO_ROTATE
    };

protected:
    virtual void createListener();
    virtual void createBullet() override;
    virtual void createShadow() override;
    virtual void bulletAndZombiesCollision() override;
    virtual bool getBulletIsEncounterWithZombie(Zombies* zombie) override;
    virtual void caveBulletInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) override;
    virtual void readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i) override;
    virtual void calculateBulletPosition();
    virtual void seekZombie();
    virtual void selectShortDistance(Zombies* zombie);
    virtual float calculateBulletAndZombie(Zombies* zombie);
    virtual float calculateAngle(Vec2 vec2);
    virtual RatateDirtection calculateDirection(Vec2 vec2);
    virtual bool bulletIsInMap();
    virtual void createBulletExplode(Zombies* zombie);

private:
    Zombies* _targetZombie;
    float _distance;
    float _locationY;
    float _distanceY;
    Vec2 _speed;
    bool _isTrack;
};