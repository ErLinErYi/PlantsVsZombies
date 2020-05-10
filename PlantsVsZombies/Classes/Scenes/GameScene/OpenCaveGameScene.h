/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.5.03
 *Email: 2117610943@qq.com
 */

#pragma once
#include "GameScene.h"
#include "spine/spine-cocos2dx.h"
#include "Based/GlobalVariable.h"

using namespace cocos2d;
using namespace spine;

class OpenCaveGameScene :public GameScene
{
public:
	CREATE_FUNC(OpenCaveGameScene);
	static Scene* createScene();

CC_CONSTRUCTOR_ACCESS:
	OpenCaveGameScene();
	~OpenCaveGameScene();
	virtual bool init() override;
	virtual void onEnter() override;
	
private:
	void openLevelData();

private:
	Global* _global;
	Director* _director;
	char* _levelName;
};