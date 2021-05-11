/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSButtonLayer.h"

class TGButtonLayer :public GSButtonLayer
{
public:
	CREATE_FUNC(TGButtonLayer);
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	TGButtonLayer();
	~TGButtonLayer();
	virtual bool init() override;

protected:
	virtual void createQuitDialog() override;
	virtual void showShovelBank() override;
	virtual void createKeyBoardListener() override;
	virtual void createPlantsCard() override;
};

