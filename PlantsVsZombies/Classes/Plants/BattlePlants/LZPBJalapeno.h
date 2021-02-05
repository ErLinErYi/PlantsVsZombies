/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZPBCherryBomb.h"

class Jalapeno :public CherryBomb
{
public:
	static Jalapeno* create(Node* node = nullptr);
    Sprite* createPlantImage() override;
    void createPlantAnimation() override;
   
CC_CONSTRUCTOR_ACCESS:
	Jalapeno(Node* node = nullptr);
	~Jalapeno();

protected:
    virtual void setPlantNoramlAnimation() override;

private:
    void plantExplode() override;
    void showExplodeAnimation() override;
    bool getZombieIsInExplodeRange(Zombies* zombie) override;
    SkeletonAnimation* showPlantAnimationAndText() override;
};