/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.11.20
 *Email: 2117610943@qq.com
 */
#include "LZDetailedListLayer.h"

DetailedList::DetailedList():
	_drawNode(nullptr)
{
}

DetailedList::~DetailedList()
{
}

bool DetailedList::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	onShowChart("普通世界简单模式", Vec2(50, 890), Vec2(500, 900), Color4F::ORANGE);
	onShowChart("普通世界困难模式", Vec2(50, 790), Vec2(500, 800), Color4F::BLUE);
	onShowChart("大地图世界简单模式", Vec2(50, 690), Vec2(500, 700), Color4F::RED);

	return true;
}

void DetailedList::onShowChart(const string& txt, const Vec2& txtPos, const Vec2& pos, Color4F color)
{
	if (!_drawNode)
	{
		_drawNode = DrawNode::create();
		this->addChild(_drawNode);
	}

	_drawNode->drawSolidRect(pos, pos + Vec2(500, -20), color);

	auto text = ui::Text::create();
	text->setString(txt);
	text->setPosition(txtPos);
	text->setFontName(GAME_FONT_NAME_1);
	text->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	text->setFontSize(25);
	this->addChild(text);
}
