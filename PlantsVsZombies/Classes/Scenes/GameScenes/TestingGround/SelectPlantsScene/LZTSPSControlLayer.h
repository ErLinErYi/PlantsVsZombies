/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.07
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSControlLayer.h"

class TSPSControlLayer :public SPSControlLayer
{
public:
	CREATE_FUNC(TSPSControlLayer);
	
CC_CONSTRUCTOR_ACCESS:
	TSPSControlLayer();
	~TSPSControlLayer();
	virtual bool init();

protected:
	virtual void createButton() override;
	virtual void showUserName() override;
	virtual void createDialog() override;
};