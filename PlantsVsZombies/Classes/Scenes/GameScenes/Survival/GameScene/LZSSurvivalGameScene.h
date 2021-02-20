/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.5.08
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGameScene.h"

class SurvivalGameScene :public GameScene
{
public:
    CREATE_FUNC(SurvivalGameScene);
    static Scene* createScene();

protected:
    virtual void controlLayer() override;
};