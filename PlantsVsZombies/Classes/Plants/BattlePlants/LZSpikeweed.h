/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZGloomShroom.h"

class Spikeweed :public GloomShroom
{
public:
	static Spikeweed* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	Spikeweed(Node* node = nullptr);
	~Spikeweed();

private:
	void createListener() override;
	void plantAttack() override;
	void zombieEatPlant(Zombies* zombie) override;
	bool getZombieIsEncounterPlant(Zombies* zombie) override;
	SkeletonAnimation* showPlantAnimationAndText() override;
};