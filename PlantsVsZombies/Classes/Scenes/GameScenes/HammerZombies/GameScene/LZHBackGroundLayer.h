/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSBackgroundLayer.h"

class HBackgroundLayer :public GSBackgroundLayer
{
public:
	CREATE_FUNC(HBackgroundLayer);
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	HBackgroundLayer();
	~HBackgroundLayer();
	virtual bool init() override;
};