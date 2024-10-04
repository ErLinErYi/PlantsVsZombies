/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.6.17
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Plants/LZPlants.h"

class Pumpkin :public Plants
{
public:
	static Pumpkin* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	
CC_CONSTRUCTOR_ACCESS:
	Pumpkin(Node* node = nullptr);
	~Pumpkin();

protected:
	virtual void checkPlantHealthPoint() override;
	SkeletonAnimation* showPlantAnimationAndText() override;
	virtual void zombieEatPlant(Zombies* zombie) override;
	virtual bool getZombieIsEncounterPlant(Zombies* zombie) override;
	virtual void readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i) override;
	virtual void setPlantOtherInformation(rapidjson::Document* levelDataDocument, char* key, int i) override;
	virtual void setZombieEatPlantControl(Zombies* zombie) override;
	virtual void setPlantScale(float scale, bool isGetInitialScale) override;

private:
	SkeletonAnimation* _plantAnimationBack;
	int _skinId;
	bool _setZombiesNoteat;

public:
	static int tagAddition;
};