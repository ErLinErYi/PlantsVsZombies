/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZBattlePlants.h"

class Zombies;

class CherryBomb :public BattlePlants
{
public:
	static CherryBomb* create(Node* node = nullptr);
    Sprite* createPlantImage() override;
    void createPlantAnimation() override;
    void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	CherryBomb(Node* node = nullptr);
	~CherryBomb();

protected:
    virtual void createListener() override;
    virtual void plantExplode() override;
    virtual SkeletonAnimation* showPlantAnimationAndText() override;
    virtual bool getPlantIsReadyExplode() const;
    virtual void explodeHurtZombies();
    virtual void showExplodeAnimation();
    virtual bool getZombieIsInExplodeRange(Zombies* zombie)override;

protected:
    bool _isReadyExplode;          // ÊÇ·ñ×¼±¸±¬Õ¨
    bool _isExplode;               // ÊÇ·ñ±¬Õ¨
    Zombies* _zombie;              // ÁÙÊ±½©Ê¬
};