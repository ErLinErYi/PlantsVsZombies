/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.14
 *Email: 2117610943@qq.com
 */
#pragma once
#include "../LZSSBackgroundLayer.h"

class SBBackgroundLayer :public SPSBackgroundLayer
{
public:
	CREATE_FUNC(SBBackgroundLayer);

CC_CONSTRUCTOR_ACCESS:
	SBBackgroundLayer();
	~SBBackgroundLayer();
	virtual bool init();

protected:
	virtual void createBackGroundEffect() override;
	virtual void showPreviewZombies() override;
};