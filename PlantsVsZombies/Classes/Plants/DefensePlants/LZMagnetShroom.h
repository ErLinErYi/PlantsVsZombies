/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.6.19
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Plants/LZPlants.h"

class MagnetShroom :public Plants
{
public:
	static MagnetShroom* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;

CC_CONSTRUCTOR_ACCESS:
	MagnetShroom(Node* node = nullptr);
	~MagnetShroom();

protected:
	virtual void cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) override;
	virtual void readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i) override;
	virtual SkeletonAnimation* showPlantAnimationAndText() override;
	virtual void createListener() override;
	virtual void determineRelativePositionPlantsAndZombies() override;
	virtual void plantAttack(Zombies* zombie);
	virtual bool getZombieIsInRange(Zombies* zombie);
	virtual void showShieldMoveAction(Zombies* zombie, const bool type);
	virtual void changeAnimation();

protected:
	bool _isCanAttack;
};
