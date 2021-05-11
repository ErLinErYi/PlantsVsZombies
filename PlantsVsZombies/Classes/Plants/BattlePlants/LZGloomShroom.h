/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.22
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZBattlePlants.h"

class Zombies;

class GloomShroom :public BattlePlants
{
public:
	static GloomShroom* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	GloomShroom(Node* node = nullptr);
	~GloomShroom();

protected:
	virtual void createListener() override;
	virtual void plantAttack(Zombies* zombie) override;
	virtual SkeletonAnimation* showPlantAnimationAndText() override;
	virtual bool getZombieIsInAttackRange(Zombies* zombie);
	virtual void plantRecovery(const string& name);

protected:
	bool _isChangeAnimation;  /* 是否改变动画 */
	bool _isAttack;           /* 是否攻击 */
	bool _isHaveZombies;      /* 是否有僵尸进入攻击范围 */
};