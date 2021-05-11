/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.19
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/LZUserWinRequirement.h"

using namespace cocos2d;

class GSRequirementLayer :public LayerColor
{
public:
    CREATE_FUNC(GSRequirementLayer);
    static Layer* addLayer();
    virtual bool init() override;
    void keyboardControl();

CC_CONSTRUCTOR_ACCESS:
    GSRequirementLayer() : _requirement(nullptr) {}
    ~GSRequirementLayer() { if(_requirement)delete _requirement; }

private:
    void showRequirement();

private:
    UserWinRequirement* _requirement;
};

/**
 * 空格键盘暂停/恢复控制
 */
extern int pressKeySpace;