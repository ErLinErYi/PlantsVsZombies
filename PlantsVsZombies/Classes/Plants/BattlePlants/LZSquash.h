/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.08
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZBattlePlants.h"

class Zombies;

class Squash :public BattlePlants
{
public:
    static Squash* create(Node* node = nullptr);
    Sprite* createPlantImage() override;
    void createPlantAnimation() override;
    void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
    Squash(Node* node = nullptr);
    ~Squash();

protected:
    virtual void createListener() override;
    virtual void plantExplode() override;
    virtual SkeletonAnimation* showPlantAnimationAndText() override;
    virtual bool getZombieIsInExplodeRange(Zombies* zombie)override;
    virtual void cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) override;
    virtual void readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i) override;
    virtual void setPlantOtherInformation(rapidjson::Document* levelDataDocument, char* key, int i) override;
    virtual void plantAttack(Zombies* zombie) override;
    virtual int checkZombiesPosition(Zombies* zombie);

protected:
    bool _isUsed;
    bool _isAttack;
    bool _delete;
    int _direction;
    float _zombiePositionX;
};