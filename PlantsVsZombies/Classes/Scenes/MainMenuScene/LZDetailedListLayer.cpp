/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.11.20
 *Email: 2117610943@qq.com
 */
#include "LZDetailedListLayer.h"
#include "Based/LZUserData.h"
#include "Based/LZPlayMusic.h"

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

	createShieldLayer(this);

	_drawNode = DrawNode::create();
	_drawNode->drawSolidRect(Vec2(20, 815), Vec2(950, 1060), Color4F(0.2f, 0.2f, 0.2f, 0.7f));
	_drawNode->drawSolidRect(Vec2(20, 550), Vec2(950, 795), Color4F(0.2f, 0.2f, 0.2f, 0.7f));
	_drawNode->drawSolidRect(Vec2(20, 285), Vec2(950, 530), Color4F(0.2f, 0.2f, 0.2f, 0.7f));
	_drawNode->drawSolidRect(Vec2(20, 20), Vec2(950, 265), Color4F(0.2f, 0.2f, 0.2f, 0.7f));

	_drawNode->drawSolidRect(Vec2(960, 20), Vec2(1900, 1060), Color4F(0.2f, 0.2f, 0.2f, 0.7f));
	this->addChild(_drawNode);

	auto jlevels = getWorldMostLevel(false, 1);
	auto klevels = getWorldMostLevel(true, 1);

	auto jlevels1 = getWorldMostLevel(false, 2);
	auto klevels1 = getWorldMostLevel(true, 2);

	auto ize = UserData::getInstance()->openIntUserData(const_cast<char*>("MOST_HAMMERZOMBIES_LEVEL_NUMBER"));

	/*onShowChart("普通世界清单", Vec2(475, 1020), "简单模式", Vec2(40, 940), "困难模式",
		Vec2(40, 860), to_string(jlevels) + " / 52", Vec2(440, 940), to_string(klevels) + " / 52", Vec2(440, 860),
		Vec2(450, 955), Vec2(450, 875), jlevels * 480 / 52.f, klevels * 480 / 52.f, Color4F::ORANGE, Color4F::YELLOW);
	onShowChart("大地图世界清单", Vec2(475, 755), "简单模式", Vec2(40, 675), "困难模式",
		Vec2(40, 595), to_string(jlevels1) + " / 52", Vec2(440, 675), to_string(klevels1) + " / 52", Vec2(440, 595),
		Vec2(450, 690), Vec2(450, 610), jlevels1 * 480 / 52.f, klevels1 * 480 / 52.f, Color4F::ORANGE, Color4F::YELLOW);*/
	/*onShowChart("小游戏清单", Vec2(475, 755), "锤僵尸模式", Vec2(40, 675), "我是僵尸模式",
		Vec2(40, 595), to_string(ize) + " / 170", Vec2(440, 675), to_string(klevels1) + " / 200", Vec2(440, 595),
		Vec2(450, 690), Vec2(450, 610), ize * 480 / 170.f, klevels1 * 480 / 200.f, Color4F::ORANGE, Color4F::YELLOW);*/
	
	string mName[3] = { "简单模式","困难模式" };
	string mNumber[3] = { to_string(jlevels) + " / 52",to_string(klevels) + " / 52" };
	Vec2 mPos[3] = { Vec2(40, 940) ,Vec2(40, 860) };
	Vec2 mNPos[3] = { Vec2(440, 940) ,Vec2(440, 860) };
	Vec2 pos[3] = { Vec2(450, 955), Vec2(450, 875) };
	float number[3] = { jlevels * 480 / 52.f, klevels * 480 / 52.f };
	Color4F color[3] = { Color4F::ORANGE, Color4F::YELLOW };
	onShowChart("普通世界清单", Vec2(475, 1020), mName, mPos, mNumber, mNPos, pos, number, color);

	mNumber[0] = to_string(jlevels1) + " / 52";
	mNumber[1] = to_string(klevels1) + " / 52";
	mPos[0] = Vec2(40, 675);
	mPos[1] = Vec2(40, 595);
	mNPos[0] = Vec2(440, 675);
	mNPos[1] = Vec2(440, 595);
	pos[0] = Vec2(450, 690);
	pos[1] = Vec2(450, 610);
	number[0] = jlevels1 * 480 / 52.f;
	number[1] = klevels1 * 480 / 52.f;
	color[0] = Color4F::ORANGE;
	color[1] = Color4F::YELLOW;
	onShowChart("大地图世界清单", Vec2(475, 755), mName, mPos, mNumber, mNPos, pos, number, color);

	onShowBackButton();

	return true;
}

void DetailedList::onShowChart(const string gName, const Vec2& gPos,
	const string& mName, const Vec2& mPos, const string& mName2, const Vec2& mPos2,
	const string& mNumber, const Vec2& mNPos, const string& mNumber2, const Vec2& mNPos2,
	const Vec2& pos, const Vec2& pos2, float number, float number2, Color4F color, Color4F color2)
{
	auto gameName = ui::Text::create();
	gameName->setString(gName);
	gameName->setPosition(gPos);
	gameName->setFontName(GAME_FONT_NAME_1);
	gameName->setFontSize(30);
	this->addChild(gameName);

	
	auto modeName = ui::Text::create();
	modeName->setString(mName);
	modeName->setPosition(mPos);
	modeName->setFontName(GAME_FONT_NAME_1);
	modeName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	modeName->setFontSize(25);
	this->addChild(modeName);

	auto modeName2 = ui::Text::create();
	modeName2->setString(mName2);
	modeName2->setPosition(mPos2);
	modeName2->setFontName(GAME_FONT_NAME_1);
	modeName2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	modeName2->setFontSize(25);
	this->addChild(modeName2);

	auto modeNumber = ui::Text::create();
	modeNumber->setString(mNumber);
	modeNumber->setPosition(mNPos);
	modeNumber->setFontName(GAME_FONT_NAME_2);
	modeNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	modeNumber->setFontSize(25);
	this->addChild(modeNumber);

	auto modeNumber2 = ui::Text::create();
	modeNumber2->setString(mNumber2);
	modeNumber2->setPosition(mNPos2);
	modeNumber2->setFontName(GAME_FONT_NAME_2);
	modeNumber2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	modeNumber2->setFontSize(25);
	this->addChild(modeNumber2);

	_drawNode->drawSolidRect(pos, pos + Vec2(480, -30), Color4F(0.5f, 0.5f, 0.5f, 0.5f));
	_drawNode->drawSolidRect(pos2, pos2 + Vec2(480, -30), Color4F(0.5f, 0.5f, 0.5f, 0.5f));

	_drawNode->drawSolidRect(pos, pos + Vec2(number, -30), color);
	_drawNode->drawSolidRect(pos2, pos2 + Vec2(number2, -30), color2);
}

void DetailedList::onShowChart(const string& gName, const Vec2& gPos, 
	string* mName, Vec2* mPos, 
	string* mNumber, Vec2* mNPos, 
	Vec2* pos, float* number, Color4F* color, int id)
{
	auto gameName = ui::Text::create();
	gameName->setString(gName);
	gameName->setPosition(gPos);
	gameName->setFontName(GAME_FONT_NAME_1);
	gameName->setFontSize(30);
	this->addChild(gameName);

	for (int i = 0; i < id; ++i)
	{
		auto modeName = ui::Text::create();
		modeName->setString(mName[i]);
		modeName->setPosition(mPos[i]);
		modeName->setFontName(GAME_FONT_NAME_1);
		modeName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		modeName->setFontSize(25);
		this->addChild(modeName);

		auto modeNumber = ui::Text::create();
		modeNumber->setString(mNumber[i]);
		modeNumber->setPosition(mNPos[i]);
		modeNumber->setFontName(GAME_FONT_NAME_2);
		modeNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		modeNumber->setFontSize(25);
		this->addChild(modeNumber);

		_drawNode->drawSolidRect(pos[i], pos[i] + Vec2(480, -30), Color4F(0.5f, 0.5f, 0.5f, 0.5f));

		_drawNode->drawSolidRect(pos[i], pos[i] + Vec2(number[i], -30), color[i]);
	}
}

int DetailedList::getWorldMostLevel(bool dif, int id)
{
	string worldFile;
	if (dif)
	{
		worldFile = StringUtils::format(Global::getInstance()->userInformation->getSystemDifCaveFileName().c_str(), id);
	}
	else
	{
		worldFile = StringUtils::format(Global::getInstance()->userInformation->getSystemCaveFileName().c_str(), id);
	}

	return max(0, UserData::getInstance()->openIntUserData(const_cast<char*>(worldFile.c_str())) - 1);
}

void DetailedList::onShowBackButton()
{
	auto button = Button::create("SeedChooser_Button2.png", "SeedChooser_Button2_Glow.png", "", cocos2d::ui::Widget::TextureResType::PLIST);
	button->setTitleText(_global->userInformation->getGameText().find("退出")->second->text);
	button->setTitleColor(Color3B(0, 255, 255));
	button->setTitleFontSize(_global->userInformation->getGameText().find("退出")->second->fontsize);
	button->setScale(2.0f);
	button->setPosition(Vec2(1750, 50));
	this->addChild(button);
	button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap2");
				break;
			case Widget::TouchEventType::ENDED:
				setMouseListenerEnable();
				deleteDialog();
				break;
			}
		});
}
