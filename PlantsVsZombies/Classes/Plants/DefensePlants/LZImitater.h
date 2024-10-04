/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Plants/LZPlants.h"

class Imitater :public Plants
{
public:
	static Imitater* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;

CC_CONSTRUCTOR_ACCESS:
	Imitater(Node* node = nullptr);
	~Imitater();

protected:
	SkeletonAnimation* showPlantAnimationAndText() override;
	virtual void createOtherPlant();
	virtual void createListener() override;
	virtual void setPlantNoramlAnimation() override;
};