/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.09
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSGameTimerLayer.h"

class TGGameTimerLayer :public GSGameTimerLayer
{
public:
	CREATE_FUNC(TGGameTimerLayer);

CC_CONSTRUCTOR_ACCESS:
	TGGameTimerLayer();
	~TGGameTimerLayer();
	virtual bool init();

public:
	virtual void createTimer() override;
};
