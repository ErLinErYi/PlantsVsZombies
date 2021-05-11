/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.30
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZBattlePlants.h"

class IceBergLettuce :public BattlePlants
{
public:
    static IceBergLettuce* create(Node* node = nullptr);
    Sprite* createPlantImage() override;
    void createPlantAnimation() override;
    void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
    IceBergLettuce(Node* node = nullptr);
    ~IceBergLettuce();

protected:
    virtual void createListener() override;
    virtual void plantExplode(Zombies* zombie);
    virtual void explodeEffectZombies(const float time);
    virtual void setZombiesActionStop(Zombies* zombie, const int &time);
    virtual bool getZombieIsInExplodeRange(Zombies* zombie) override;
    virtual SkeletonAnimation* showPlantAnimationAndText() override;
    virtual void cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) override;
    virtual void readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i) override;

private:
    bool _isHaveZombies;
    bool _isChanged;
    int _excludeZombieZorder;
    Vec2 _excludeZombiePosition;
    Zombies* _excludeZombie;
};