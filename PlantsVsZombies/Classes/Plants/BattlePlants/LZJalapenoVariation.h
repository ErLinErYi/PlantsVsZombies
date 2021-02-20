/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.24
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZJalapeno.h"

class JalapenoVariation :public Jalapeno
{
public:
    static JalapenoVariation* create(Node* node = nullptr);
    Sprite* createPlantImage() override;
    void createPlantAnimation() override;

CC_CONSTRUCTOR_ACCESS:
    JalapenoVariation(Node* node = nullptr);
    ~JalapenoVariation();

private:
    void showExplodeAnimation() override;
    bool getZombieIsInExplodeRange(Zombies* zombie) override;
    SkeletonAnimation* showPlantAnimationAndText() override;
};