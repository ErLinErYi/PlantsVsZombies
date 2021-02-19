/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Scenes/SelectPlantsScene/LZSSBackgroundLayer.h"

using namespace cocos2d;

class GameType;

class GSBackgroundLayer :public SPSBackgroundLayer
{
public:
	CREATE_FUNC(GSBackgroundLayer);
	static void backgroundRunAction();
	virtual void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	GSBackgroundLayer();
	~GSBackgroundLayer();
	virtual bool init();

protected:
	virtual void setBackgroundImagePosition();

protected:
	Global* _global;
};