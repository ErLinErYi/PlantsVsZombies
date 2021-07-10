/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.04
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSGameTimerLayer.h"

class IGameTimerLayer :public GSGameTimerLayer
{
public:
	CREATE_FUNC(IGameTimerLayer);

CC_CONSTRUCTOR_ACCESS:
	IGameTimerLayer();
	~IGameTimerLayer();
	virtual bool init() override;

public:
	virtual void createTimer() override;
};