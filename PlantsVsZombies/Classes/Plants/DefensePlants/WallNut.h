/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Plants/Plants.h"

class WallNut :public Plants
{
public:
	static WallNut* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;

CC_CONSTRUCTOR_ACCESS:
	WallNut(Node* node = nullptr);
	~WallNut();

private:
	void checkPlantHealthPoint() override;

private:
	int _skinId;
};