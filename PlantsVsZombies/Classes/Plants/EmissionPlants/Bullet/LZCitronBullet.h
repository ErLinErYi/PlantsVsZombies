/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.27
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZBullet.h"

class CitronBullet :public Bullet
{
public:
    /**
     *´´½¨×Óµ¯
     */
    virtual void createBullet() override;

    /**
     *×Óµ¯Óë½©Ê¬Åö×²¼ì²â
     */
    virtual void bulletAndZombiesCollision() override;

    void setAnimationId(const int tag);
    int getAnimationId();

CC_CONSTRUCTOR_ACCESS:
    CitronBullet(Node* node, int id);
    ~CitronBullet();

private:
    void createShadow() override;
    void caveBulletInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) override;
    void readBulletInformation(rapidjson::Document* levelDataDocument, char* key, int i) override;
    void readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i) override;
    void createExplode();
    void attackZombies(Zombies* zombie);
    void splashDamageZombies(Zombies* exceptZombie);
    bool getZombieInExplodeRange(Zombies* zombie);

private:
    int _animationId;
};