/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class TGGameOverLayer :public LayerColor
{
public:
	CREATE_FUNC(TGGameOverLayer);

CC_CONSTRUCTOR_ACCESS:
	TGGameOverLayer();
	~TGGameOverLayer();
	virtual bool init() override;

protected:
	virtual void setQuitGame();
};

