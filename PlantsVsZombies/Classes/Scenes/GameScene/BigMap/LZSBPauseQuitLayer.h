/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.15
 *Email: 2117610943@qq.com
 */

#pragma once
#include "../LZSGPauseQuitLayer.h"

class BMPauseQuitLayer :public GSPauseQuitLayer
{
public:
	CREATE_FUNC(BMPauseQuitLayer);

CC_CONSTRUCTOR_ACCESS:
	BMPauseQuitLayer();
	~BMPauseQuitLayer();
	virtual bool init();

protected:
	//virtual void createButton(const Vec2& vec2, const std::string name, PauseQuitLayer_Button type);
	//virtual void createDialog() override;
	//virtual void popSceneAnimation();
	//virtual void showPrompt();
	//virtual void openHandBook();
	virtual void setRestart() override;
	//virtual void setQuitGame();
	//virtual void keyDescription();
	//virtual void returnGame();
};
