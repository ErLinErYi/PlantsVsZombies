/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include"PeaShooter.h"

class FirePeaShooter :public PeaShooter
{
public:
	static FirePeaShooter* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	FirePeaShooter(Node* node = nullptr);
	~FirePeaShooter();

private:
	void plantAttack(Zombies* zombie) override;
	void createPea() override;
};