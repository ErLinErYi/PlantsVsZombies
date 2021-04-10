/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSControlLayer.h"

class TGControlLayer :public GSControlLayer
{
public:
	CREATE_FUNC(TGControlLayer);
	virtual void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	TGControlLayer();
	~TGControlLayer();
	virtual bool init();

protected:
	virtual void initData() override;
	virtual void createSchedule() override;
	virtual void createZombies() override;
	virtual bool controlRefurbishZombies() override;
	virtual void controlRefurbishMusicAndText() override;
	virtual void selectPlantsPreviewImage() override;
	virtual void mouseLeftButtonDownControl() override;
	virtual void mouseRightButtonDownControl() override;
};