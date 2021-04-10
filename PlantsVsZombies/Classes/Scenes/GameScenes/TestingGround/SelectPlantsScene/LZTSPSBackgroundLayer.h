/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.07
 *Email: 2117610943@qq.com
 */
#pragma once

#include "Scenes/GameScenes/Adventure/GameScene/LZAGSBackgroundLayer.h"

class TSPSBackgroundLayer :public GSBackgroundLayer
{
public:
	CREATE_FUNC(TSPSBackgroundLayer);

CC_CONSTRUCTOR_ACCESS:
	TSPSBackgroundLayer();
	~TSPSBackgroundLayer();
	virtual bool init();

protected:
	virtual void createBackGroundEffect() override;
	virtual void showPreviewZombies() override;
};
