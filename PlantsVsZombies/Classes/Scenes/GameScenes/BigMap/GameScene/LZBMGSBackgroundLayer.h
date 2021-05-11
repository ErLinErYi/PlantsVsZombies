/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.13
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSBackgroundLayer.h"

class BMBackgroundLayer :public GSBackgroundLayer
{
public:
	CREATE_FUNC(BMBackgroundLayer);
	
CC_CONSTRUCTOR_ACCESS:
	BMBackgroundLayer();
	~BMBackgroundLayer();
	virtual bool init() override;

protected:
	virtual void createBackGroundEffect() override;
	virtual void setBackgroundImagePosition() override;
};