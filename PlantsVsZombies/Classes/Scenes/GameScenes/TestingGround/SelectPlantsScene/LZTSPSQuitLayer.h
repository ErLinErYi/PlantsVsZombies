/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.07
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSQuitLayer.h"

class TSPSQuitLayer :public SPSQuitLayer
{
public:
	CREATE_FUNC(TSPSQuitLayer);

CC_CONSTRUCTOR_ACCESS:
	TSPSQuitLayer() {}
	virtual bool init();

protected:
	virtual void createButton(const Vec2& vec2, const std::string name, PauseQuitLayer_Button type) override;
};
