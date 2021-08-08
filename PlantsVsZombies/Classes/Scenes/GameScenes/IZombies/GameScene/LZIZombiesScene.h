/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.02
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGameScene.h"

using namespace cocos2d;

class IZombiesScene :public GameScene
{
public:
	CREATE_FUNC(IZombiesScene);

CC_CONSTRUCTOR_ACCESS:
	IZombiesScene();
	~IZombiesScene();
	virtual bool init() override;

protected:
	virtual void controlPlayMusic() override;
	virtual void backgroundLayer() override;
	virtual void informationLayer() override;
	virtual void buttonLayer() override;
	virtual void controlLayer() override;
	virtual void animationLayer() override;
	virtual void gameTimerLayer() override;
};