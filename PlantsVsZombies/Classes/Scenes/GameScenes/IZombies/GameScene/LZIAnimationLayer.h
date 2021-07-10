/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.02
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"

class IAnimationLayer :public GSAnimationLayer
{
public:
	CREATE_FUNC(IAnimationLayer);
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }
	virtual void createZombies(ZombiesType type, float xPosition);

CC_CONSTRUCTOR_ACCESS:
	IAnimationLayer();
	~IAnimationLayer();
	virtual bool init() override;

protected:
	virtual void gameMainLoop() override;
	virtual void gameMainLoop(float delta) override;
	virtual void zombiesEventUpdate(float delta) override;
	virtual void showBrain();
	virtual void showFlower();

public:
	struct Brain
	{
		Sprite* brain;
		bool enable;
		int row;
	};
	static list<Brain> BrainGroup;
};