/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.14
 *Email: 2117610943@qq.com
 */
#pragma once
#include "../LZSGGameScene.h"

class BigMapGameScene :public GameScene
{
public:
	CREATE_FUNC(BigMapGameScene);

CC_CONSTRUCTOR_ACCESS:
	BigMapGameScene();
	~BigMapGameScene();
	virtual bool init() override;

protected:
	virtual void createScrollView();
	virtual void showGameLayer() override;
	virtual void backgroundLayer() override;
	virtual void controlLayer() override;
	virtual void animationLayer() override;
	virtual void informationLayer() override;
	virtual void goodsLayer() override;
	virtual void buttonLayer() override;

public:
	static extension::ScrollView* scrollView;

protected:
	Layer* _scrollLayer;
};