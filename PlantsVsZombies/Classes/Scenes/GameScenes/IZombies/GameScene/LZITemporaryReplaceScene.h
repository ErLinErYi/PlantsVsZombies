/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.09.21
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/HammerZombies/GameScene/LZHContinueOrExitLayer.h"

class IContinueOrExitLayer :public HContinueOrExitLayer
{
public:
	CREATE_FUNC(IContinueOrExitLayer);
	static IContinueOrExitLayer* createScene(int type);

CC_CONSTRUCTOR_ACCESS:
	IContinueOrExitLayer();
	~IContinueOrExitLayer();
	virtual bool init() override;

protected:
	virtual void createButton(const string& text, const Vec2& pos, const int buttonType) override;
	virtual void createText();
};
