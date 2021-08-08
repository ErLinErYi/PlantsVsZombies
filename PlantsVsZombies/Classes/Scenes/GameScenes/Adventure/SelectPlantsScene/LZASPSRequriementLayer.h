/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.3.9
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;

class UserWinRequirement;
class SPSRequriementLayer :public LayerColor
{
public:
    CREATE_FUNC(SPSRequriementLayer);

CC_CONSTRUCTOR_ACCESS:
    SPSRequriementLayer();
    ~SPSRequriementLayer();
    virtual bool init() override;

protected:
    virtual void createDialog();

protected:
    UserWinRequirement* _requirement;
};