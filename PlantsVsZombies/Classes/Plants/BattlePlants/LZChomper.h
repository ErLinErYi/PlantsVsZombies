/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.16
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZBattlePlants.h"

class Chomper :public BattlePlants
{
public:
    static Chomper* create(Node* node = nullptr);
    Sprite* createPlantImage() override;
    void createPlantAnimation() override;
    void determineRelativePositionPlantsAndZombies() override;

    int getChewTime() const;
    bool getIsCanEatZombie() const;
    void setChewTime(const int time);
    void setIsCanEatZombie(const bool isCanEat);

CC_CONSTRUCTOR_ACCESS:
    Chomper(Node* node = nullptr);
    ~Chomper();

protected:
    virtual void createListener() override;
    virtual void plantAttack(Zombies* zombie) override;
    virtual bool getZombieIsEncounterPlant(Zombies* zombie) override;
    virtual SkeletonAnimation* showPlantAnimationAndText() override;

private:
    void cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) override;
    void readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i) override;
    bool isCanKillZombieOnce(Zombies* zombie);
    void chomperKillZombie();
    void chomperHurtZombie();
    void zombieRelieveReserveKill(Zombies* zombie);

private:
    bool _isCanEat;           // 是否可以吃僵尸
    int _chewTime;            // 大嘴花咀嚼时间
    Zombies* _attackZombie;   // 被攻击僵尸
};
