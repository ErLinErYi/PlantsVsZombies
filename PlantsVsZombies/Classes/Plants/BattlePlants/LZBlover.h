/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.6.18
 *Email: 2117610943@qq.com
 */
#pragma once
#include "LZBattlePlants.h"

class Blover :public BattlePlants
{
public:
	static Blover* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;

CC_CONSTRUCTOR_ACCESS:
	Blover(Node* node = nullptr);
	virtual ~Blover();

protected:
	virtual void createListener() override;
	SkeletonAnimation* showPlantAnimationAndText() override;
	virtual void determineRelativePositionPlantsAndZombies() override;
	virtual void plantAttack(Zombies* zombie) override;
};