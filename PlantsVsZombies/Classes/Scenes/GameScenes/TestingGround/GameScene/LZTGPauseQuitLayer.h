/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/HammerZombies/GameScene/LZHPauseQuiteLayer.h"

class TGPauseQuitLayer :public HPauseQuitLayer
{
public:
	CREATE_FUNC(TGPauseQuitLayer);

CC_CONSTRUCTOR_ACCESS:
	TGPauseQuitLayer();
	~TGPauseQuitLayer();
	virtual bool init() override;

protected:
	virtual void createDialog() override;
	virtual void setRestart() override;
	virtual void setQuitGame() override;
	virtual void popSceneAnimation() override;
};
