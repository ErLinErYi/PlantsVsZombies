/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Emal: 2117610943@qq.com
 */

#pragma once
#include "LZPEEmissionPlants.h"

#define _MAX_ 0xffffff

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

protected:
	virtual void createBullet() override;
	virtual void plantAttack(Zombies* zombie) override;
	virtual void plantAttack(Zombies* zombie, const string& animationName);
	virtual SkeletonAnimation* showPlantAnimationAndText() override;
	virtual void determineRelativePositionPlantsAndZombies(const string& animationName);

protected:
	float _distance;          // 植物与最近的僵尸的距离
	Vec2 _zombiePostion;      // 临时挑选出来的僵尸的位置
	float _zombieSpeed;       // 临时挑选出来的僵尸的速度
	float _zombieHeight;      // 临时挑选出来的僵尸的高度
	Zombies* _zombie;         // 存储临时挑选出来的僵尸
};