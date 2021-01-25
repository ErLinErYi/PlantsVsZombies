/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.17
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZPEBPea.h"
#include "Zombies/LZZZombies.h"

class IcePea :public Pea
{
public:
    static void setZombieActionSlow(Zombies* zombie);
    static void setZombieActionRecovery(Zombies* zombie);

CC_CONSTRUCTOR_ACCESS:
    IcePea(Node* node);
    ~IcePea();

protected:
    virtual void createShadow() override;
    virtual void bulletAndZombiesCollision() override;
    virtual void readBulletInformation(rapidjson::Document* levelDataDocument, char* key, int i) override;
    virtual void readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i) override;
    virtual void createPeaExplode() override;
    virtual void createPeaExplode(Zombies* zombie);
    virtual void icePeaExplodeEffect(Zombies* zombie);
};