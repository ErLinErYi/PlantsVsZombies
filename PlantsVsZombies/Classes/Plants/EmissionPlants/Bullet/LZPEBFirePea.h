/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.19
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZPEBPea.h"

class FirePea :public Pea
{
public:
    void createBullet() override;

CC_CONSTRUCTOR_ACCESS:
    FirePea(Node* node);
    ~FirePea();

protected:
    virtual void createShadow(Vec2 position = Vec2::ZERO) override;
    virtual void bulletAndZombiesCollision() override;
    virtual void readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i) override;
    virtual void attackZombies(Zombies* zombie) override;
    virtual void createPeaExplode() override;
    virtual void splashDamageZombies(Zombies* exceptZombie);
    virtual void recoveryFrozenZombie(Zombies* zombie);
};