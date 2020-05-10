/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.5.08
 *Email: 2117610943@qq.com
 */

#pragma once
#include "../SelectPlantsScene.h"

class SurvivalSelectScene :public SelectPlantsScene
{
public:
    CREATE_FUNC(SurvivalSelectScene);
    static Scene* createScene();

CC_CONSTRUCTOR_ACCESS:
    virtual bool init() override;

protected:
    virtual void replaceScene() override;
    virtual void createBackgroundLayer() override;
    virtual void createSelectPlantsDialog() override;
    virtual void eventUpdate(float time) override;
};