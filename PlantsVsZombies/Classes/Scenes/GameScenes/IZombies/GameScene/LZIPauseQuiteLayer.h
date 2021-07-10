/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.02
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseQuitLayer.h"

class IPauseQuitLayer :public GSPauseQuitLayer
{
public:
	CREATE_FUNC(IPauseQuitLayer);

CC_CONSTRUCTOR_ACCESS:
	IPauseQuitLayer();
	~IPauseQuitLayer();
	virtual bool init() override;

protected:
	virtual void setRestart() override;
	virtual void setQuitGame() override;
};