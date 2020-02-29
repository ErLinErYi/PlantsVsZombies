/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.19
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Pea.h"

class FirePea:public Pea
{
public:
    void createBullet() override;

CC_CONSTRUCTOR_ACCESS:
    FirePea(Node* node);
    ~FirePea();

private:
    void bulletInit() override;
    void createShadow() override;
};