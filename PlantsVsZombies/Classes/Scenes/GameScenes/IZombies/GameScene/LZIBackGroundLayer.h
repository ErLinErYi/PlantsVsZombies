/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.02
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSBackgroundLayer.h"

class IBackgroundLayer :public GSBackgroundLayer
{
public:
	CREATE_FUNC(IBackgroundLayer);
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	IBackgroundLayer();
	~IBackgroundLayer();
	virtual bool init() override;
};