/**
* Copyright(c) 2020 LZ.All Right Reserved
* Author : LZ
* Date : 2021.1.17
* Email : 2117610943@qq.com
*/

#pragma once
#include "LZPeaShooter.h"

class Zombies;
enum class SoundEffectType;

class IcePeaShooter :public PeaShooter
{
public:
	static IcePeaShooter* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	IcePeaShooter(Node* node = nullptr);
	virtual ~IcePeaShooter();

protected:
	virtual void createBullet() override;
	virtual void plantAttack(Zombies* zombie) override;
	virtual SkeletonAnimation* showPlantAnimationAndText() override;
};
