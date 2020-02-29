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
	HelpScene():_global(Global::getInstance()), size(Director::getInstance()->getWinSize()){}
	virtual bool init() override;

private:
	void createBackground();
	void createText();
	void createGithubUrl();
	void createButton();

private:
	Global* _global;
	const Size size;
};
