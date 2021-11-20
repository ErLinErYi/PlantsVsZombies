/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.11.20
 *Email: 2117610943@qq.com
 */
#pragma once
#include "cocos2d.h"
#include "Based/LZDialog.h"

using namespace cocos2d;

class DetailedList:public Dialog
{
public:
	CREATE_FUNC(DetailedList);

CC_CONSTRUCTOR_ACCESS:
	DetailedList();
	~DetailedList();
	virtual bool init() override;

private:
	void onShowChart(const string& txt, const Vec2& txtPos, const Vec2& pos, Color4F color);

private:
	DrawNode* _drawNode;
};
