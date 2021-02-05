/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.26
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZPEEmissionPlants.h"

class Zombies;
enum class SoundEffectType;

class StarFruit :public EmissionPlants
{
public:
	static StarFruit* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	StarFruit(Node* node = nullptr);
	~StarFruit();

protected:
	virtual void createBullet() override;
	virtual void plantAttack(Zombies* zombie) override;
	virtual SkeletonAnimation* showPlantAnimationAndText() override;
	virtual void plantEmission(const string& plantAnimation);
	virtual void plantRecovery(const string& plantAnimation);

private:
	bool zombieInPlantAttackRange(Zombies* zombie);
	bool zombieInObliqueDirection(float& disX, float& disY);

private:
	Bullet* _bulletAnimation;  // ×Óµ¯¶¯»­
};