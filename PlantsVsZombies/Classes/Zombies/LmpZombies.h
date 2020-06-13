/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.26
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Zombies.h"

using namespace cocos2d;

class LmpZombies :public Zombies
{
public:
	static LmpZombies* create(Node* node = nullptr);
	virtual void createZombie() override;
	virtual void createPreviewZombie() override;
    virtual void playZombieSoundEffect() override;
    virtual void playZombiesDieAnimation() override;

CC_CONSTRUCTOR_ACCESS:
	LmpZombies(Node* node = nullptr);

private:
    void zombieInjuredEventUpdate() override;
    void setZombiePrimaryInjure() override;
    void setZombieSecondaryInjure() override;
};