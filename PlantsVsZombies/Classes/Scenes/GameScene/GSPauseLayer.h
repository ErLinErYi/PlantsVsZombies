/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.3.8
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "spine/spine-cocos2dx.h"

#include "Based/GlobalVariable.h"

using namespace cocos2d;
using namespace spine;
using namespace cocos2d::ui;

class GSPauseLayer :public LayerColor
{
public:
    CREATE_FUNC(GSPauseLayer);
    static Layer* addLayer();

CC_CONSTRUCTOR_ACCESS:
    GSPauseLayer();
    ~GSPauseLayer();
    virtual bool init();

private:
    void createShieldLayer();
    void createDialog();
    void showAniamtions();
    void showButton();

private:
    Scale9Sprite* _levelObjiectives;
    Global* _global;
};