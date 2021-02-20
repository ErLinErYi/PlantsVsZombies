/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.29
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZPeaShooter.h"

class ThreePeaShooter :public PeaShooter
{
public:
	static ThreePeaShooter* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	ThreePeaShooter(Node* node = nullptr);
	~ThreePeaShooter();

protected:
	virtual void createBullet() override;
	virtual void plantAttack(Zombies* zombie) override;
	virtual bool getZombieIsSameLineWithPlant(Zombies* zombie) override;
	virtual SkeletonAnimation* showPlantAnimationAndText() override;
};