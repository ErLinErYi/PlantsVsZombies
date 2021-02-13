/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */
#pragma once
#include "../LZSGGameScene.h"

using namespace cocos2d;

class HammerZombiesScene :public GameScene
{
public:
	CREATE_FUNC(HammerZombiesScene);

CC_CONSTRUCTOR_ACCESS:
	HammerZombiesScene();
	~HammerZombiesScene();
	virtual bool init() override;

protected:
	virtual void controlPlayMusic() override;
	virtual void backgroundLayer() override;
	virtual void informationLayer() override;
	virtual void buttonLayer() override;
	virtual void controlLayer() override;
	virtual void animationLayer() override;
	virtual void gameTimerLayer() override;
	virtual void pauseGame() override;
	virtual void goodsLayer() override;
};