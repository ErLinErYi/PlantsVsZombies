/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.15
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSInformationLayer.h"

class BMInformationLayer :public GSInformationLayer
{
public:
	CREATE_FUNC(BMInformationLayer);
	virtual void addLayer(Node* node, const int order, const string& name) override { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	BMInformationLayer();
	~BMInformationLayer();
	virtual bool init() override;

protected:
	virtual void showGameType() override;
	virtual void showUserText() override;
};
