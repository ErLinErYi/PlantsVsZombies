/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.04
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZCabbage.h"

class WaterMelonBullet :public Cabbage
{
public:
	virtual void createBullet() override;
	virtual void bulletAndZombiesCollision() override;
	
CC_CONSTRUCTOR_ACCESS:
	WaterMelonBullet(Node* node = nullptr);
	~WaterMelonBullet();

protected:
	virtual void bulletAttackHurtZombies(Zombies* zombie) override;
	virtual void splashDamageZombies(Zombies* exceptZombie);
	virtual bool getZombieInExplodeRange(Zombies* zombie);
};