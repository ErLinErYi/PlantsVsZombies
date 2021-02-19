/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.14
 *Email: 2117610943@qq.com
 */
#pragma once
#include "../LZSSSelectPlantsScene.h"

class BSelectPlantsScene :public SelectPlantsScene
{
public:
	CREATE_FUNC(BSelectPlantsScene);

CC_CONSTRUCTOR_ACCESS:
	BSelectPlantsScene();
	~BSelectPlantsScene();
	virtual bool init();

protected:
	virtual void replaceScene() override;
	virtual void createBackgroundLayer() override;
	virtual void controlShowRequirement() override;
	virtual void selectPlantsCallBack() override;
	virtual void eventUpdate(SPSEventType eventType) override;
};