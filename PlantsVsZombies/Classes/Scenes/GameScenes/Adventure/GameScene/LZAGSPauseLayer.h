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

#include "Based/LZDialog.h"
#include "Based/LZGlobalVariable.h"

using namespace cocos2d;
using namespace spine;
using namespace cocos2d::ui;

class GSPauseLayer :public Dialog
{
public:
    CREATE_FUNC(GSPauseLayer);
    static Layer* addLayer();

CC_CONSTRUCTOR_ACCESS:
    GSPauseLayer();
    ~GSPauseLayer();
    virtual bool init() override;

private:
    void createDialog();
    void showAniamtions();
    void showButton();

private:
    cocos2d::ui::Scale9Sprite* _levelObjiectives;
    Global* _global;
};