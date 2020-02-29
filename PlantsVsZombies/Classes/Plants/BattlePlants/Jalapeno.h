/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "CherryBomb.h"

class Jalapeno :public CherryBomb
{
public:
	static Jalapeno* create(Node* node = nullptr);
    Sprite* createPlantImage() override;
    void createPlantAnimation() override;
   
CC_CONSTRUCTOR_ACCESS:
	Jalapeno(Node* node = nullptr);
	~Jalapeno();

private:
    void plantExplode() override;
    void showExplodeAnimation() override;
    bool getZombieIsInExplodeRange(Zombies* zombie) const override;
};