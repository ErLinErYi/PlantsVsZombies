/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.8
 *Email: 2117610943@qq.com
 */

#pragma once

#include "CommonZombies.h"

class CommonFlagZombies :public CommonZombies
{
public:
    static CommonFlagZombies* create(Node* node = nullptr);
    virtual void createZombie() override;
    virtual void createPreviewZombie() override;
    virtual void playZombieSoundEffect() override;

CC_CONSTRUCTOR_ACCESS:
    CommonFlagZombies(Node* node = nullptr);
};
