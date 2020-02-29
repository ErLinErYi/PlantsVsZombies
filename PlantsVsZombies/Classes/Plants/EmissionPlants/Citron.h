/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.27
 *Email: 2117610943@qq.com
 */

#pragma once
#include "EmissionPlants.h"

class Citron :public EmissionPlants
{
public:
	static Citron* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	Citron(Node* node = nullptr);
	~Citron();

protected:
	virtual void createBullet();
	virtual void plantAttack(Zombies* zombie) override;
	virtual void plantEmission();
	virtual void plantRecovery(const string& plantAnimation);

private:
	void createListener();
	int calculateGreatEvocationProbability();

private:
	int _attackInterval;              // 攻击间隔
	int _animationId;                 // 动画编号
	bool _isBaginReady;               // 是否已经开始准备
	bool _readyFinished;              // 准备完成
};