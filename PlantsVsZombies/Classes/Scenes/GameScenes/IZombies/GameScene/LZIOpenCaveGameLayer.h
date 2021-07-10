/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.04
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZIZombiesScene.h"
#include "spine/spine-cocos2dx.h"
#include "Based/LZGlobalVariable.h"

using namespace cocos2d;

class IOpenCaveGameLayer :public IZombiesScene
{
public:
	CREATE_FUNC(IOpenCaveGameLayer);
	static bool isReadFile;

CC_CONSTRUCTOR_ACCESS:
	IOpenCaveGameLayer();
	~IOpenCaveGameLayer();
	virtual bool init() override;
	virtual void onEnterTransitionDidFinish() override;

protected:
	virtual void openLevelData();

protected:
	Global* _global;
	Director* _director;
	char* _levelName;
};