/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.9.02
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/TestingGround/SelectPlantsScene/LZTSPSRequirementLayer.h"

class IRequriementLayer :public TSPSRequriementLayer
{
public:
    CREATE_FUNC(IRequriementLayer);

CC_CONSTRUCTOR_ACCESS:
    IRequriementLayer();
    ~IRequriementLayer();
   
protected:
    virtual void createText() override;
};