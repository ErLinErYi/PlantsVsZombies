/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Scenes/GameScene/GSPauseQuitLayer.h"

using namespace cocos2d;

class SPSQuitLayer :public GSPauseQuitLayer
{
public:
	CREATE_FUNC(SPSQuitLayer);
	
CC_CONSTRUCTOR_ACCESS:
	SPSQuitLayer() : _shieldListener(nullptr){}
	virtual bool init();

protected:
	virtual void createButton(const Vec2& vec2, const std::string name, PauseQuitLayer_Button type) override;

private:
	void createDialog() override;  

private:
	EventListenerTouchOneByOne* _shieldListener;
};