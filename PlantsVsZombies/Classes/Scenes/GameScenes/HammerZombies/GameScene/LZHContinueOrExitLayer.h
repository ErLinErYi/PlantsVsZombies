/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.09.21
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/LZGlobalVariable.h"

using namespace cocos2d;

class HContinueOrExitLayer :public Scene
{
public:
	CREATE_FUNC(HContinueOrExitLayer);
	static HContinueOrExitLayer* createScene(int type);

CC_CONSTRUCTOR_ACCESS:
	HContinueOrExitLayer();
	~HContinueOrExitLayer();
	virtual bool init() override;

protected:
	virtual void createBackground();
	virtual void createButton(const string& text, const Vec2& pos, const int buttonType);
	virtual void createText();

protected:
	Global* _global;
	static int type;
};
