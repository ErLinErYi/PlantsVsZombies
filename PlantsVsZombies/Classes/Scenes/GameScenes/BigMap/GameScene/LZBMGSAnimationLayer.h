/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.14
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"

class BMAnimationLayer :public GSAnimationLayer
{
public:
	CREATE_FUNC(BMAnimationLayer);
	static BMAnimationLayer* create(Node* node);
	virtual void addLayer(Node* node, const int order, const string& name) override { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	BMAnimationLayer(Node* node = nullptr);
	~BMAnimationLayer();
	virtual bool init() override;

protected:
	virtual void showCars() override;
	virtual void plantPlants() override;
	virtual void createZombies() override;
	virtual void zombiesEventUpdate(float delta) override;
};