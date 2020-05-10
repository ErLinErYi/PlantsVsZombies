/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "BattlePlants.h"

class PotatoMine :public BattlePlants
{
public:
	static PotatoMine* create(Node* node = nullptr);
	void setBreakGround(const float time);
	float getBreakGround() const;
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	PotatoMine(Node* node = nullptr);
	~PotatoMine();

private:
	void plantExplode() override;
	void zombieEatPlant(Zombies* zombie) override;
	void createListener();
	void explodeHurtZombies();
	bool getZombieIsInExplodeRange(Zombies* zombie);
	
private:
	bool _isReady;                 // 是否准备好
	bool _isExplodeFinished;       // 是否爆炸完成
	bool _isBeginExplode;          // 是否开始爆炸
	bool _isCanKillZombies;        // 是否可以杀手僵尸
	bool _isKillZombiesFinished;   // 是否杀死僵尸结束
	float _breakGround;            // 破土时间
};