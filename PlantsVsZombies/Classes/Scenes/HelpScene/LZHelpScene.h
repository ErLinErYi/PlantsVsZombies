/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.1
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/LZGlobalVariable.h"

using namespace cocos2d;

class HelpScene :public Scene
{
public:
	static Scene* createHelpScene();
	CREATE_FUNC(HelpScene);

CC_CONSTRUCTOR_ACCESS:
	HelpScene():_global(Global::getInstance()), _size(Director::getInstance()->getWinSize()){}
	virtual bool init() override;

private:
	void createBackground();
	void createText();
	void openUrl(string name, string url, Vec2 postion);
	void createButton();
	void createParticle();
	void addScrollView();
	void addMouseEvent();
	
private:
	Global* _global;
	const Size _size;
	ui::ScrollView* _textScrollView;
	float _plantCardRollingDistance;           /* 计算鼠标滚动距离 */
};
