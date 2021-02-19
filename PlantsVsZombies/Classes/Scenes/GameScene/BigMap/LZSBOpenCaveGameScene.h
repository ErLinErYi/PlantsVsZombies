/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.16
 *Email: 2117610943@qq.com
 */
#pragma once
#include "LZSBBigMapGameScene.h"

class BMOpenCaveGameScene :public BigMapGameScene
{
public:
	CREATE_FUNC(BMOpenCaveGameScene);

CC_CONSTRUCTOR_ACCESS:
	BMOpenCaveGameScene();
	~BMOpenCaveGameScene();
	virtual bool init() override;
	virtual void onEnter() override;

protected:
	virtual void openLevelData();

protected:
	char* _levelName;
};
