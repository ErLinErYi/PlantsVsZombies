/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.07
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASelectPlantsScene.h"

class TSelectPlantsScene :public SelectPlantsScene
{
public:
	CREATE_FUNC(TSelectPlantsScene);

CC_CONSTRUCTOR_ACCESS:
	TSelectPlantsScene();
	~TSelectPlantsScene();
	virtual bool init();

protected:
	virtual void createControlLayer() override;
	virtual void replaceScene() override;
	virtual void createBackgroundLayer() override;
	virtual void controlShowRequirement() override;
	virtual void createSelectPlantsDialog() override;
};
