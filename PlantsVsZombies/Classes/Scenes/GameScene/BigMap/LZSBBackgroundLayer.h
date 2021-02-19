/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.13
 *Email: 2117610943@qq.com
 */

#pragma once
#include "../LZSGBackgroundLayer.h"

class BMBackgroundLayer :public GSBackgroundLayer
{
public:
	CREATE_FUNC(BMBackgroundLayer);
	
CC_CONSTRUCTOR_ACCESS:
	BMBackgroundLayer();
	~BMBackgroundLayer();
	virtual bool init();

protected:
	virtual void createBackGroundEffect();
	virtual void setBackgroundImagePosition();
};