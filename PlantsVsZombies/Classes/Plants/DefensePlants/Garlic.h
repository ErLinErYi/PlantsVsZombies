/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "../Plants.h"

class Garlic :public Plants
{
public:
	static Garlic* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	Garlic(Node* node = nullptr);
	~Garlic();

private:
	void zombieEatPlant(Zombies* zombie) override;
	void zombieRecoveryMove(Zombies* zombie) override;
	void checkPlantHealthPoint() override;
	bool getPlantIsSurvive() const override;
	bool getzombieIsEncounterPlant(Zombies* zombie) const override;
	void changeZombiePositionY(Zombies* zombie);

private:
	int _canEatNumbers;             // 可以被吃的次数
	int _currentCanEatNumbers;      // 当前可以被吃的次数
	int _animationId;               // 动画编号
};