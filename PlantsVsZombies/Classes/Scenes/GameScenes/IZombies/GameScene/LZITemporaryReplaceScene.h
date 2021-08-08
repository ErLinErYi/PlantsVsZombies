/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.21
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class TemporaryReplaceScene :public Scene
{
public:
	CREATE_FUNC(TemporaryReplaceScene);
	
CC_CONSTRUCTOR_ACCESS:
	TemporaryReplaceScene();
	~TemporaryReplaceScene();
	virtual void onEnterTransitionDidFinish() override;
};