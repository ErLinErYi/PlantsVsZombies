/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"

class HAnimationLayer :public GSAnimationLayer
{
public:
	CREATE_FUNC(HAnimationLayer);
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }
	virtual void createZombies(const float mulSpeed, const float addSpeed,const unsigned typeNumber);

CC_CONSTRUCTOR_ACCESS:
	HAnimationLayer();
	~HAnimationLayer();
	virtual bool init();

protected:
	virtual void gameMainLoop(float delta) override;
	virtual void zombiesEventUpdate(float delta) override;
};