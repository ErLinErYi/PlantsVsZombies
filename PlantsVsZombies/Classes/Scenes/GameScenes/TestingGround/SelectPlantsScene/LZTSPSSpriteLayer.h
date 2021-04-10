/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSSpriteLayer.h"

class TSPSSpriteLayer : public SPSSpriteLayer
{
public:
	CREATE_FUNC(TSPSSpriteLayer);
	virtual Text* showPlantsInformation(Button* button, bool showHeart = false) override;

CC_CONSTRUCTOR_ACCESS:
	TSPSSpriteLayer();
	~TSPSSpriteLayer();
	virtual bool init();

protected:
	virtual void controlPlantCanSelect(Button* button, int priority) override;
};