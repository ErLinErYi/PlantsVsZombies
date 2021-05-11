/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.16
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZZombies.h"

class BalloonZombies :public Zombies
{
public:
    static BalloonZombies* create(Node* node = nullptr);
    virtual void createZombie() override;
    virtual void createPreviewZombie() override;
    virtual void playZombieSoundEffect() override;
    virtual void playZombiesAshesAnimation() override;
    virtual bool getZombieIsEatPlants() override;
    virtual bool getZombieIsCanBeAttack() override;

CC_CONSTRUCTOR_ACCESS:
    BalloonZombies(Node* node = nullptr);

private:
    void zombieInjuredEventUpdate() override;
    void setZombiePrimaryInjure() override;
    void setZombieSecondaryInjure() override;
};