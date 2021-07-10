/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGameScene.h"

class TestingGroundScene :public GameScene
{
public:
	CREATE_FUNC(TestingGroundScene);

CC_CONSTRUCTOR_ACCESS:
	TestingGroundScene();
	~TestingGroundScene();
	virtual bool init() override;

protected:
	virtual void showGameLayer() override;
	virtual void controlPlayMusic() override;
	virtual void backgroundLayer() override;
	virtual void informationLayer() override;
	virtual void buttonLayer() override;
	virtual void controlLayer() override;
	virtual void animationLayer() override;
	virtual void gameTimerLayer() override;
};
