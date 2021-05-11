/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.20
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/LZGlobalVariable.h"

using namespace cocos2d;

class GSGameTimerLayer :public Layer
{
public:
	CREATE_FUNC(GSGameTimerLayer);

CC_CONSTRUCTOR_ACCESS:
	GSGameTimerLayer();
	~GSGameTimerLayer();
	virtual bool init() override;

public:
	virtual void createTimer();

public:
	static int breakThroughTime;

protected:
	Global* _global;
};