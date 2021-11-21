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

	auto hammer = UserData::getInstance()->openIntUserData(const_cast<char*>("MOST_HAMMERZOMBIES_LEVEL_NUMBER"));
	auto testGround = UserData::getInstance()->openIntUserData(const_cast<char*>("TESTINGGROUND"));
	auto ize = UserData::getInstance()->openIntUserData(const_cast<char*>("IZOMBIES_MOST_LEVEL"));
	
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

	mName[0] = "锤僵尸模式";
	mName[1] = "植物试炼场";
	mName[2] = "我是僵尸模式";
	mNumber[0] = to_string(hammer) + " / 170";
	mNumber[1] = to_string(testGround) + " / 200";
	mNumber[2] = to_string(ize) + " / 251";
	mPos[0] = Vec2(40, 440);
	mPos[1] = Vec2(40, 380);
	mPos[2] = Vec2(40, 320);
	mNPos[0] = Vec2(440, 440);
	mNPos[1] = Vec2(440, 380);
	mNPos[2] = Vec2(440, 320);
	pos[0] = Vec2(450, 455);
	pos[1] = Vec2(450, 395);
	pos[2] = Vec2(450, 335);
	number[0] = min(hammer * 480 / 170.f, 480.f);
	number[1] = min(testGround * 480 / 200.f,480.f);
	number[2] = min(ize * 480 / 251.f, 480.f);
	color[0] = Color4F::ORANGE;
	color[1] = Color4F::YELLOW;
	color[2] = Color4F::BLUE;
	onShowChart("小游戏清单", Vec2(475, 490), mName, mPos, mNumber, mNPos, pos, number, color, 3);

	onShowOtherThings();

	onShowBackButton();

	return true;
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
	gameName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
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

void DetailedList::onShowOtherThings()
{
	_drawNode->drawRect(Vec2(40, 40), Vec2(930, 245), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	_drawNode->drawLine(Vec2(40, 177), Vec2(930, 177), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	_drawNode->drawLine(Vec2(40, 109), Vec2(930, 109), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	_drawNode->drawLine(Vec2(445, 245), Vec2(445, 40), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	_drawNode->drawLine(Vec2(300, 245), Vec2(300, 109), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	_drawNode->drawLine(Vec2(830, 245), Vec2(830, 109), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
}
