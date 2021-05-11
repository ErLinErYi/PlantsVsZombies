/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/LZGlobalVariable.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;

class SPSControlLayer :public Layer
{
public:
	CREATE_FUNC(SPSControlLayer);
	static bool isShowJumpLevelButton();
	static int getLevelBreakThroughNumbers();
	static ui::Text* showCoinNumbers(Node* node);
	static ui::Text* showDieZombiesNumbers(Node* node);

CC_CONSTRUCTOR_ACCESS:
	SPSControlLayer();
	~SPSControlLayer();
	virtual bool init() override;

protected:
	virtual void createButton();
	virtual void showUserName();
	virtual void createDialog();
	
protected:
	Global* _global;
};