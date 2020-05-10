/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"

#include "Based/GlobalVariable.h" 

#define SCROLLRIGHTFINISHED -1010
#define SCROLLLEFTFINISHED  -220

using namespace cocos2d;
using namespace cocos2d::extension;

class SPSSpriteLayer;
class SPSControlLayer;

class SelectPlantsScene :public Scene
{
public:
	static Scene* createScene(); 
	CREATE_FUNC(SelectPlantsScene);

CC_CONSTRUCTOR_ACCESS:
	SelectPlantsScene();
	~SelectPlantsScene();
	virtual bool init();

protected:
	/**
	 *创建文字
	 */
	virtual void createReadyText(const std::string& name, const int& id);

	/**
	 *文字回调
	 */
	virtual void readyTextCallBack(Node* node, const std::string& name, const int& id);

	/**
	 *切换场景
	 */
	virtual void replaceScene();

	/**
	 *创建背景层
	 */
	virtual void createBackgroundLayer();

	/**
	 *创建控制层
	 */
	virtual void createControlLayer();

	/**
	 *事件更新
	 */
	virtual void eventUpdate(float Time);

	/**
	 *创建植物选择对话框
	 */
	virtual void createSelectPlantsDialog();

	/**
	 *控制显示要求
	 */
	virtual void controlShowRequirement();

	/**
	 *植物选择回调函数
	 */
	virtual void selectPlantsCallBack();

protected:
	Layer* _scrollLayer;
	Director* _director;
	Global* _global;
	SPSControlLayer* _controlLayer;
	SPSSpriteLayer* _spriteLayer;
	extension::ScrollView* _scrollView;
};