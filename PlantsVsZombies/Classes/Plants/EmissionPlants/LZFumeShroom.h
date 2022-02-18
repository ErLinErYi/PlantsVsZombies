/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.08
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZEmissionPlants.h"

class Zombies;
enum class SoundEffectType;

class FumeShroom :public EmissionPlants
{
public:
	static FumeShroom* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	FumeShroom(Node* node = nullptr);
	virtual ~FumeShroom();

protected:
	virtual void createBullet() override;
	virtual void plantAttack(Zombies* zombie) override;
	virtual SkeletonAnimation* showPlantAnimationAndText() override;
	virtual bool getZombieIsTheFrontOfPlant(Zombies* zombie) override;
	virtual void plantEmission(const string& plantAnimation);
	virtual void plantRecovery(const string& plantAnimation);
};