/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "spine/spine.h"
#include "spine/spine-cocos2dx.h"
#include "Scenes/SelectPlantsScene/SelectPlantsScene.h"
#include "Based/GlobalVariable.h"

using namespace cocos2d;
using namespace spine;

class GameScene :public SelectPlantsScene
{
public:
	static Scene* createScene();
	CREATE_FUNC(GameScene);

CC_CONSTRUCTOR_ACCESS:
	GameScene();
	~GameScene();
	virtual bool init();

private:
	void controlPlayMusic();
	void backgroundLayer();
	void buttonLayer();
	void informationLayer();
	void controlLayer();
	void animationLayer();

private:
	Global* _global;
	Director* _director;
};