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
#include "Scenes/SelectPlantsScene/LZSSSelectPlantsScene.h"
#include "Based/LZBGlobalVariable.h"

using namespace cocos2d;
using namespace spine;

class GameScene :public SelectPlantsScene
{
public:
	CREATE_FUNC(GameScene);

CC_CONSTRUCTOR_ACCESS:
	GameScene();
	~GameScene();
	virtual bool init() override;
	
protected:
	virtual void showGameLayer();
	virtual void controlPlayMusic();
	virtual void backgroundLayer();
	virtual void buttonLayer();
	virtual void informationLayer();
	virtual void controlLayer();
	virtual void animationLayer();
	virtual void goodsLayer();
	virtual void gameTimerLayer();
	virtual void pauseGame();

public:
	static bool isRunGameScene;
	
protected:
	Global* _global;
	Director* _director;
};