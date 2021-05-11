/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.15
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseQuitLayer.h"

class BMPauseQuitLayer :public GSPauseQuitLayer
{
public:
	CREATE_FUNC(BMPauseQuitLayer);

CC_CONSTRUCTOR_ACCESS:
	BMPauseQuitLayer();
	~BMPauseQuitLayer();
	virtual bool init() override;

protected:
	virtual void setRestart() override;
};
