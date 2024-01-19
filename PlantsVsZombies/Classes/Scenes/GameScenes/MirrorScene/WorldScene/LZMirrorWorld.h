/**
 *Copyright (c) 2024 LZ.All Right Reserved
 *Author : LZ
 *Date: 2024.1.15
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Based/LZGlobalVariable.h"
#include "Scenes/GameScenes/Adventure/WorldScene/LZModernWorld.h"

using namespace cocos2d;

class MirrorWorld :public ModernWorld
{
public:
	CREATE_FUNC(MirrorWorld);

CC_CONSTRUCTOR_ACCESS:
	MirrorWorld();
	virtual ~MirrorWorld();
	virtual bool init() override;

protected:
	//virtual void createBackground() override;
	virtual void createMirrorWorld();
};
