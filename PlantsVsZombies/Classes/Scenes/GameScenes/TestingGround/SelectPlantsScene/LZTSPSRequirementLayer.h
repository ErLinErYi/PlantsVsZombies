/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.09
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Based/LZDialog.h"

class TSPSRequriementLayer :public Dialog
{
public:
    CREATE_FUNC(TSPSRequriementLayer);

CC_CONSTRUCTOR_ACCESS:
    TSPSRequriementLayer();
    ~TSPSRequriementLayer();
    virtual bool init();

protected:
    virtual void createDialog();
    virtual void createText();
    virtual void showButton();

protected:
    Sprite* _objiectives;
};