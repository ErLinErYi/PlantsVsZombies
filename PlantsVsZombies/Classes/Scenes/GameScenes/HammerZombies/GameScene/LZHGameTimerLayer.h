
/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSGameTimerLayer.h"

class HGameTimerLayer :public GSGameTimerLayer
{
public:
	CREATE_FUNC(HGameTimerLayer);

CC_CONSTRUCTOR_ACCESS:
	HGameTimerLayer();
	~HGameTimerLayer();
	virtual bool init() override;

public:
	virtual void createTimer() override;
};
