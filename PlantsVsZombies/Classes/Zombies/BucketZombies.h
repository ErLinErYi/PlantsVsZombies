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

class BucketZombies :public Zombies
{
public:
	static BucketZombies* create(Node* node = nullptr);
	virtual void createZombie() override;
	virtual void createPreviewZombie() override;
    virtual void playZombieSoundEffect() override;

CC_CONSTRUCTOR_ACCESS:
	BucketZombies(Node* node = nullptr);

private:
    void zombieInjuredEventUpdate() override;
    void setZombieHeadShieldThirdInjure(const string& oldName, const string& newName) override;
};