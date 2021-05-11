/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"

class TGAnimationLayer :public GSAnimationLayer
{
public:
	CREATE_FUNC(TGAnimationLayer);
	static TGAnimationLayer* create(Node* node);
	virtual void addLayer(Node* node, const int order, const string& name) override { node->addChild(this, order, name); }
	virtual void createZombies(const unsigned typeNumber);

CC_CONSTRUCTOR_ACCESS:
	TGAnimationLayer(Node* node = nullptr);
	~TGAnimationLayer();
	virtual bool init() override;

protected:
	virtual void zombiesEventUpdate(float delta) override;
	virtual void showCars() override;
	virtual void judgeZombiesWin(list<Zombies*>::iterator& zombie);
};

