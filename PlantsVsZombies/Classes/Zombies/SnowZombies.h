/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.23
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Zombies.h"

using namespace cocos2d;

class SnowZombies :public Zombies
{
public:
    static SnowZombies* create(Node* node = nullptr);
    virtual void createZombie() override;
    virtual void createPreviewZombie() override;
    virtual void playZombieSoundEffect() override;

CC_CONSTRUCTOR_ACCESS:
    SnowZombies(Node* node = nullptr);

private:
    void zombieInjuredEventUpdate() override;
    void setZombiePrimaryInjure() override;
    void setZombieSecondaryInjure() override;

private:
    default_random_engine _random;
    random_device _device;
};