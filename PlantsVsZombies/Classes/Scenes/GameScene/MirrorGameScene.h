/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.17
 *Email: 2117610943@qq.com
 */

#pragma once
#include "GameScene.h"

class MirrorGameScene :public GameScene
{
public:
    CREATE_FUNC(MirrorGameScene);
    static Scene* createScene();

CC_CONSTRUCTOR_ACCESS:
    virtual bool init() override;

protected:
    virtual void createMirrorWorld();
};