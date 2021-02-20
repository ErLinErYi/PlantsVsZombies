/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.14
 *Email: 2117610943@qq.com
 */
#pragma once

#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSBackgroundLayer.h"

class BMSPSBackgroundLayer :public SPSBackgroundLayer
{
public:
	CREATE_FUNC(BMSPSBackgroundLayer);

CC_CONSTRUCTOR_ACCESS:
	BMSPSBackgroundLayer();
	~BMSPSBackgroundLayer();
	virtual bool init();

protected:
	virtual void createBackGroundEffect() override;
	virtual void showPreviewZombies() override;
};