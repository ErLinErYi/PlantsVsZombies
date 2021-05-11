/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.04
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZWaterMelonBullet.h"

class WinterMelonBullet :public WaterMelonBullet
{
public:
	virtual void createBullet() override;
	virtual void bulletAndZombiesCollision() override;

CC_CONSTRUCTOR_ACCESS:
	WinterMelonBullet(Node* node = nullptr);
	~WinterMelonBullet();

protected:
	virtual void bulletAttackHurtZombies(Zombies* zombie) override;
	virtual void splashDamageZombies(Zombies* exceptZombie) override;
	virtual void winterMelonExplodeEffect(Zombies* zombie);
};