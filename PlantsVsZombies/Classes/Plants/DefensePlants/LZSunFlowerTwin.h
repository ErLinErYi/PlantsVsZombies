/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.3
 *Emal: 2117610943@qq.com
 */

#pragma once
#include "LZSunFlower.h"

class SunFlowerTwin :public SunFlower
{
public:
	static SunFlowerTwin* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;

CC_CONSTRUCTOR_ACCESS:
	SunFlowerTwin(Node* node = nullptr);
	~SunFlowerTwin();

protected:
	virtual void createListener() override;
	virtual void playAnimation() override;
	virtual void createSuns() override;
	virtual SkeletonAnimation* showPlantAnimationAndText() override;
};