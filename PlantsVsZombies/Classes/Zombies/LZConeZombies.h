/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.26
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "LZZombies.h"

using namespace cocos2d;

class ConeZombies :public Zombies
{
public:
	static ConeZombies* create(Node* node = nullptr);
	virtual void createZombie() override;
	virtual void createPreviewZombie() override;
	virtual Sprite* createPreviewZombieImage() override;
    virtual void playZombieSoundEffect() override;

CC_CONSTRUCTOR_ACCESS:
	ConeZombies(Node* node = nullptr);

protected:
	virtual void zombieInjuredEventUpdate() override;
	virtual void setZombieHeadShieldThirdInjure(const string& oldName, const string& newName) override;
};