/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.08
 *Email: 2117610943@qq.com
 */

#pragma once
#include "ConeZombies.h"

using namespace cocos2d;

class ConeFlagZombies :public ConeZombies
{
public:
    static ConeFlagZombies* create(Node* node = nullptr);
    virtual void createZombie() override;
    virtual void createPreviewZombie() override;
    virtual void playZombieSoundEffect() override;

CC_CONSTRUCTOR_ACCESS:
    ConeFlagZombies(Node* node = nullptr);
};