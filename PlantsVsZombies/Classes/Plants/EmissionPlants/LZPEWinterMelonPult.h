/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.3
 *Emal: 2117610943@qq.com
 */

#pragma once

#pragma once
#include "LZPEWaterMelonPult.h"

class Bullet;

class WinterMelonPult :public WaterMelonPult
{
public:
	static WinterMelonPult* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	WinterMelonPult(Node* node = nullptr);
	~WinterMelonPult();

protected:
	virtual void createBullet() override;
	virtual void plantAttack(Zombies* zombie) override;
	virtual SkeletonAnimation* showPlantAnimationAndText() override;
};