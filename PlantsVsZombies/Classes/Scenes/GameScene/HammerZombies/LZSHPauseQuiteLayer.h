/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */
#pragma once
#include "../LZSGPauseQuitLayer.h"

class HPauseQuitLayer :public GSPauseQuitLayer
{
public:
	CREATE_FUNC(HPauseQuitLayer);

CC_CONSTRUCTOR_ACCESS:
	HPauseQuitLayer();
	~HPauseQuitLayer();
	virtual bool init();

protected:
	virtual void createDialog() override;
	virtual void createButton(const Vec2& vec2, const std::string name, PauseQuitLayer_Button type) override;
	virtual void setRestart() override;
	virtual void setQuitGame() override;
};