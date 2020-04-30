/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Emal: 2117610943@qq.com
 */

#pragma once
#include "EmissionPlants.h"

class Bullet;

class CabbagePult :public EmissionPlants
{
public:
	static CabbagePult* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	CabbagePult(Node* node = nullptr);
	~CabbagePult();

private:
	void plantAttack(Zombies* zombie) override;
	void createCabbage();

private:
	bool _isCreateCabbage;    // 是否创建据卷心菜子弹
	float _distance;          // 植物与最近的僵尸的距离
	Vec2 _zombiePostion;      // 临时挑选出来的僵尸的位置
	float _zombieSpeed;       // 临时挑选出来的僵尸的速度
	Zombies* _zombie;         // 存储临时挑选出来的僵尸
};