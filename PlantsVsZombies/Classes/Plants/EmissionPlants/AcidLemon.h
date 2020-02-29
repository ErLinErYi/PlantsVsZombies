/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.25
 *Email: 2117610943@qq.com
 */

#pragma once
#include "EmissionPlants.h"

class AcidLemonShooter :public EmissionPlants
{
public:
	static AcidLemonShooter* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	AcidLemonShooter(Node* node = nullptr);
	~AcidLemonShooter();

protected:
	virtual void createAcidLemon();
	virtual void plantAttack(Zombies* zombie) override;
	virtual void plantEmission(const string& plantAnimation);
	virtual void plantRecovery(const string& plantAnimation);

protected:
	bool _isCreateAcidLemon;
};