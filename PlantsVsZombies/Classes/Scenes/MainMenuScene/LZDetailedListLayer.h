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
	void onShowChart(const string gName, const Vec2& gPos,
		const string& mName, const Vec2& mPos, const string& mName2, const Vec2& mPos2,
		const string& mNumber, const Vec2& mNPos, const string& mNumber2, const Vec2& mNPos2,
		const Vec2& pos, const Vec2& pos2, float number, float number2, Color4F color, Color4F color2);
	void onShowChart(const string& gName, const Vec2& gPos,
		string* mName, Vec2* mPos,
		string* mNumber, Vec2* mNPos,
		Vec2* pos, float* number, Color4F* color, int id = 2);

	int getWorldMostLevel(bool dif, int id);
	void onShowBackButton();

private:
	DrawNode* _drawNode;
};
