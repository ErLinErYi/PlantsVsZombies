/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/GlobalVariable.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;

class SPSControlLayer :public Layer
{
public:
	CREATE_FUNC(SPSControlLayer);

	/**
	 *添加到主场景中
	 */
	void addLayer(Node* node) { node->addChild(this, 0, "ControlLayer"); }

CC_CONSTRUCTOR_ACCESS:
	SPSControlLayer();
	~SPSControlLayer();
	virtual bool init();

private:
	void createButton();
	void showUserName();
	void createDialog();
	
private:
	Global* _global;
	LayerColor* _quitLayer;
};