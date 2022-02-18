/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.7
 *Email: 2117610943@qq.com
 */
#pragma once
#include "LZEmissionPlants.h"

class Zombies;
enum class SoundEffectType;

class CatTail :public EmissionPlants
{
public:
	static CatTail* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	CatTail(Node* node = nullptr);
	virtual ~CatTail();

protected:
	virtual void createBullet() override;
	virtual void plantAttack(Zombies* zombie) override;
	virtual SkeletonAnimation* showPlantAnimationAndText() override;
	virtual void plantEmission(const string& plantAnimation);
	virtual void plantRecovery(const string& plantAnimation);
};