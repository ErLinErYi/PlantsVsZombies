/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Plants/Plants.h"

class Bullet;

class Torchwood :public Plants
{
public:
	static Torchwood* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	Torchwood(Node* node = nullptr);
	~Torchwood();

private:
	void judgeTorchwoodAndPeaPosition();                   // 豌豆与火炬树桩位置判断
	bool getPeaIsSameLineWithTorchwood(Bullet* bullet);    // 获取豌豆与火炬树桩是否在同一行
	bool getPeaIsEncounterTorchwood(Bullet* bullet);       // 获取豌豆与火炬树桩是否相遇
	bool getBulletIsPea(Bullet* bullet);                   // 获取子弹是否是豌豆
};