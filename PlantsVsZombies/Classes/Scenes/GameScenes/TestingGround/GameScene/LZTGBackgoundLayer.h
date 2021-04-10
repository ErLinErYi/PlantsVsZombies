/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#pragma once
#include "../SelectPlantsScene/LZTSPSBackgroundLayer.h"

class TGBackgroundLayer :public TSPSBackgroundLayer
{
public:
	CREATE_FUNC(TGBackgroundLayer);
	virtual void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	TGBackgroundLayer();
	~TGBackgroundLayer();
	virtual bool init();
};