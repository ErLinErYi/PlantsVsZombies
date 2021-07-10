/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.03
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/HammerZombies/GameScene/LZHGameEndLayer.h"

class IGameEndLayer :public HGameEndLayer
{
public:
	CREATE_FUNC(IGameEndLayer);
	
CC_CONSTRUCTOR_ACCESS:
	IGameEndLayer();
	~IGameEndLayer();
	virtual bool init() override;

protected:
	virtual void rewardThing() override;
	virtual void rewardCoin(Button* button) override;
	virtual void showFailText() override;
};