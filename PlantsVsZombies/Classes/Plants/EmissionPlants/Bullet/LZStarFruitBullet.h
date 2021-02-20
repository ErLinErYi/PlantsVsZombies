/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.27
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZBullet.h"

class StarFruitBullet :public Bullet
{
public:
    enum class StarFruitBulletDirection
    {
        Up,                 // 上
        Down,               // 下
        Rear,               // 后方
        DiagonallyAbove,    // 斜上方
        ObliquelyBelow,     // 斜下方
        null
    };

public:
    virtual void createBullet() override;
    virtual void bulletAndZombiesCollision() override;
    virtual void setBulletAction(StarFruitBulletDirection action);

CC_CONSTRUCTOR_ACCESS:
    StarFruitBullet(Node* node = nullptr);
    ~StarFruitBullet();

protected:
    virtual void createShadow() override;
    virtual bool getBulletIsEncounterWithZombie(Zombies* zombie) override;
    virtual void caveBulletInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) override;
    virtual void readBulletInformation(rapidjson::Document* levelDataDocument, char* key, int i) override;
    virtual void readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i) override;
    virtual void bulletFadeOut();
    virtual void createBulletExplode(Zombies* zombie);
    virtual void attackZombies(Zombies* zombie);

private:
    StarFruitBulletDirection _direction;
    float _locationY;
    float _distance;
};
