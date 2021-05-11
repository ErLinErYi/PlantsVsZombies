/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.01.09
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "spine/spine-cocos2dx.h"

#include "Scenes/GameScenes/Adventure/GameScene/LZAGSGameEndLayer.h"
#include "Based/LZGlobalVariable.h"

using namespace cocos2d;
using namespace spine;
using namespace cocos2d::ui;

class SPSJumpLevelLayer :public GSGameEndLayer
{
public:
    CREATE_FUNC(SPSJumpLevelLayer);

CC_CONSTRUCTOR_ACCESS:
    SPSJumpLevelLayer();
    ~SPSJumpLevelLayer();
    virtual bool init() override;

private:
    void createDialog();
    void createText();
    void showButton();
    void jumpLevel();
    void createButton(Vec2& vec2, string title, const int id);

private:
    Sprite* _jumpLevelDialog;
    Global* _global;
};