/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.27
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Bullet.h"

class CitronBullet :public Bullet
{
public:
    /**
     *创建子弹
     */
    virtual void createBullet() override;

    /**
     *子弹与僵尸碰撞检测
     */
    virtual void bulletAndZombiesCollision() override;

    void setAnimationId(const int tag);
    int getAnimationId();

CC_CONSTRUCTOR_ACCESS:
    CitronBullet(Node* node, int id);
    ~CitronBullet();

private:
    void bulletInit() override;
    void createShadow() override;
    bool getBulletIsSameLineWithZombie(Zombies* zombie) override;
    void createExplode();
    void attackZombies();
    float getZombieInExplodeRange(Zombies* zombie);

private:
    int _animationId;
};