/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.09
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZBullet.h"

class FumeShroomBullet :public Bullet
{
public:
    virtual void createBullet() override;
    virtual void bulletAndZombiesCollision() override;

CC_CONSTRUCTOR_ACCESS:
    FumeShroomBullet(Node* node);
    virtual ~FumeShroomBullet();

protected:
    virtual bool getBulletIsEncounterWithZombie(Zombies* zombie) override;
    virtual void bulletAttackHurtZombies(Zombies* zombie) override;
};