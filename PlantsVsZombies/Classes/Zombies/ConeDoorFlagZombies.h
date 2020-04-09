/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.08
 *Email: 2117610943@qq.com
 */

#pragma once
#include "ConeDoorZombies.h"

class ConeDoorFlagZombies :public ConeDoorZombies
{
public:
    static ConeDoorFlagZombies* create(Node* node = nullptr);
    virtual void createZombie() override;
    virtual void createPreviewZombie() override;
    virtual void playZombieSoundEffect() override;

CC_CONSTRUCTOR_ACCESS:
    ConeDoorFlagZombies(Node* node = nullptr);

private:
    void setAttachment();
};
