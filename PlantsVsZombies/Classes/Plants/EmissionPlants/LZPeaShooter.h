﻿/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZEmissionPlants.h"

class Zombies;
enum class SoundEffectType;

class PeaShooter :public EmissionPlants
{
public:
	static PeaShooter* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;
	
CC_CONSTRUCTOR_ACCESS:
	PeaShooter(Node* node = nullptr);
	~PeaShooter();

protected:
	virtual void createBullet() override;
	virtual void plantAttack(Zombies* zombie) override;
	virtual SkeletonAnimation* showPlantAnimationAndText() override;
	virtual void plantEmission(const string& plantAnimation);
	virtual void plantRecovery(const string& plantAnimation);
};