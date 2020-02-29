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
#include "SPSSpriteLayer.h"
#include "Based/GlobalVariable.h" 
#include "Based/UserWinRequirement.h"

#define SCROLLRIGHTFINISHED -1010
#define SCROLLLEFTFINISHED  -220

using namespace cocos2d;
using namespace cocos2d::extension;

class SelectPlantsScene :public Scene
{
public:
	static Scene* createScene(); 
	CREATE_FUNC(SelectPlantsScene);

CC_CONSTRUCTOR_ACCESS:
	SelectPlantsScene();
	~SelectPlantsScene();
	virtual bool init();

private:
	/**
	 *创建背景层
	 */
	void createBackgroundLayer();

	/**
	 *创建控制层
	 */
	void createControlLayer();

	/**
	 *事件更新
	 */
	void eventUpdate(float Time);

	/**
	 *创建植物选择对话框
	 */
	void createSelectPlantsDialog();

	/**
	 *控制显示要求
	 */
	void controlShowRequirement();

	/**
	 *植物选择回调函数
	 */
	void selectPlantsCallBack();

	/**
	 *创建文字
	 */
	void createReadyText(const std::string& name, const int& id);

	/**
	 *文字回调
	 */
	void readyTextCallBack(Node* node, const std::string& name, const int& id);

private:
	Layer* _controlLayer;
	Layer* _scrollLayer;
	Layer* _spriteLayer;
	Director* _director;
	Global* _global;
	SPSSpriteLayer* _spriteLayer_;
	extension::ScrollView* _scrollView;
	UserWinRequirement* _requirement;
};