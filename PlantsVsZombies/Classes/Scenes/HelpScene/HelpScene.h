/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.1
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/GlobalVariable.h"

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
	void createGithubUrl();
	void createButton();
	void addScrollView();
	void addMouseEvent();

private:
	Global* _global;
	const Size _size;
	ui::ScrollView* _textScrollView;
	float _plantCardRollingDistance;           /* º∆À„ Û±Íπˆ∂Øæ‡¿Î */
};
