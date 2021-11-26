/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.11.20
 *Email: 2117610943@qq.com
 */
#include "LZDetailedListLayer.h"
#include "Based/LZUserData.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZDefine.h"

DetailedList::DetailedList():
	_drawNode(nullptr),
	_modernPLevel(0),
	_modernKLevel(0),
	_bigMapPLevel(0),
	_bigMapKLevel(0),
	_hammerLevel(0),
	_testGroundLevel(0),
	_izeLevel(0)
{
}

DetailedList::~DetailedList()
{
}

bool DetailedList::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createShieldLayer(this);

	_drawAction = DrawNode::create();
	this->addChild(_drawAction,1);
	_drawNode = DrawNode::create();
	_drawNode->drawSolidRect(Vec2(20, 815), Vec2(950, 1060), Color4F(0.2f, 0.2f, 0.2f, 0.7f));
	_drawNode->drawSolidRect(Vec2(20, 550), Vec2(950, 795), Color4F(0.2f, 0.2f, 0.2f, 0.7f));
	_drawNode->drawSolidRect(Vec2(20, 285), Vec2(950, 530), Color4F(0.2f, 0.2f, 0.2f, 0.7f));
	_drawNode->drawSolidRect(Vec2(20, 20), Vec2(950, 265), Color4F(0.2f, 0.2f, 0.2f, 0.7f));

	_drawNode->drawSolidRect(Vec2(970, 20), Vec2(1900, 1060), Color4F(0.2f, 0.2f, 0.2f, 0.7f));
	this->addChild(_drawNode);

	_modernPLevel = getWorldMostLevel(false, 1);
	_modernKLevel = getWorldMostLevel(true, 1);

	_bigMapPLevel = getWorldMostLevel(false, 2);
	_bigMapKLevel = getWorldMostLevel(true, 2);

	_hammerLevel = UserData::getInstance()->openIntUserData(const_cast<char*>("MOST_HAMMERZOMBIES_LEVEL_NUMBER"));
	_testGroundLevel = UserData::getInstance()->openIntUserData(const_cast<char*>("TESTINGGROUND"));
	_izeLevel = UserData::getInstance()->openIntUserData(const_cast<char*>("IZOMBIES_MOST_LEVEL"));
	
	
	string mName[3] = { GAME_TEXT("简单模式"),GAME_TEXT("噩梦模式") };
	string mNumber[3] = { to_string(_modernPLevel) + " / 52",to_string(_modernKLevel) + " / 52" };
	Vec2 mPos[3] = { Vec2(40, 940) ,Vec2(40, 860) };
	Vec2 mNPos[3] = { Vec2(440, 940) ,Vec2(440, 860) };
	Vec2 pos[3] = { Vec2(450, 955), Vec2(450, 875) };
	float number[3] = { _modernPLevel / 52.f * 480, _modernKLevel / 52.f * 480 };
	Color4F color[3] = { Color4F(0,1,0,0.5f), Color4F(1,0,0,0.5f) };
	onShowChart(GAME_TEXT("普通世界清单"), Vec2(475, 1020), mName, mPos, mNumber, mNPos, pos, number, color);

	mNumber[0] = to_string(_bigMapPLevel) + " / 52";
	mNumber[1] = to_string(_bigMapKLevel) + " / 52";
	mPos[0] = Vec2(40, 675);
	mPos[1] = Vec2(40, 595);
	mNPos[0] = Vec2(440, 675);
	mNPos[1] = Vec2(440, 595);
	pos[0] = Vec2(450, 690);
	pos[1] = Vec2(450, 610);
	number[0] = _bigMapPLevel / 52.f * 480;
	number[1] = _bigMapKLevel / 52.f * 480;
	color[0] = Color4F(1, 0.5f, 0, 0.5f);
	color[1] = Color4F(0.35f, 0, 0.35f, 0.5f);
	onShowChart(GAME_TEXT("大地图世界清单"), Vec2(475, 755), mName, mPos, mNumber, mNPos, pos, number, color);

	mName[0] = GAME_TEXT("锤僵尸模式原译");
	mName[1] = GAME_TEXT("植物试炼场模式");
	mName[2] = GAME_TEXT("我是僵尸模式");
	mNumber[0] = to_string(_hammerLevel) + " / 170";
	mNumber[1] = to_string(_testGroundLevel) + " / 200";
	mNumber[2] = to_string(_izeLevel) + " / 251";
	mPos[0] = Vec2(40, 440);
	mPos[1] = Vec2(40, 380);
	mPos[2] = Vec2(40, 320);
	mNPos[0] = Vec2(440, 440);
	mNPos[1] = Vec2(440, 380);
	mNPos[2] = Vec2(440, 320);
	pos[0] = Vec2(450, 455);
	pos[1] = Vec2(450, 395);
	pos[2] = Vec2(450, 335);
	number[0] = max(min(_hammerLevel / 170.f * 480, 480.f), 0.f);
	number[1] = max(min(_testGroundLevel / 200.f * 480, 480.f), 0.f);
	number[2] = max(min(_izeLevel / 251.f * 480, 480.f), 0.f);
	color[0] = Color4F(0, 0, 1, 0.5f);
	color[1] = Color4F(1, 1, 0, 0.5f);
	color[2] = Color4F(1, 0, 0.5f, 0.5f);
	onShowChart(GAME_TEXT("小游戏清单"), Vec2(475, 490), mName, mPos, mNumber, mNPos, pos, number, color, 3);

	onShowOtherThings();
	onShowSumList();

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
		modeNumber->setColor(Color3B(0, 255, 255));
		modeNumber->setFontSize(25);
		this->addChild(modeNumber);

		_drawNode->drawSolidRect(pos[i], pos[i] + Vec2(480, -30), Color4F(0.5f, 0.5f, 0.5f, 0.5f));

		auto a = pos[i];
		auto b = color[i];
		for (int j = 0; j <= number[i]; j+=10)
		{
			runAction(Sequence::create(DelayTime::create(0.001f*j), CallFunc::create([=]() {
				//_drawAction->clear();
				_drawAction->drawSolidRect(a, a + Vec2(j, -30), b); 
				}), nullptr));
		}
		//_drawNode->drawSolidRect(pos[i], pos[i] + Vec2(number[i], -30), color[i]);
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
	button->setTitleText(GAME_TEXT("退出"));
	button->setTitleColor(Color3B(0, 255, 255));
	button->setTitleFontSize(GAME_TEXT_SIZE("退出"));
	button->setScale(2.0f);
	button->setPosition(Vec2(1785, 50));
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
	_drawNode->drawLine(Vec2(785, 245), Vec2(785, 109), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	/*_drawNode->drawLine(Vec2(687, 177), Vec2(687, 109), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	_drawNode->drawLine(Vec2(600, 177), Vec2(600, 109), Color4F(0.5f, 0.5f, 0.5f, 0.7f));
	_drawNode->drawLine(Vec2(843, 177), Vec2(843, 109), Color4F(0.5f, 0.5f, 0.5f, 0.7f));*/

	onShowTextData(GAME_TEXT("闯关失败次数"), to_string(_global->userInformation->getBreakThroughNumbers()), Vec2(170, 210), Vec2(372, 210));
	onShowTextData(GAME_TEXT("僵尸死亡总量"), to_string(_global->userInformation->getKillZombiesNumbers()), Vec2(170, 142), Vec2(372, 142));
	onShowTextData(GAME_TEXT("当前金币数量"), to_string(_global->userInformation->getCoinNumbers()), Vec2(615, 210), Vec2(857, 210));
	onShowTextData(GAME_TEXT("使用植物总量"), to_string(_global->userInformation->getUsePlantsNumbers()), Vec2(615, 142), Vec2(857, 142));
	/*onShowTextData(GAME_TEXT("使用植物数量"), to_string(_global->userInformation->getUsePlantsNumbers()), Vec2(522, 142), Vec2(643, 142));
	onShowTextData(GAME_TEXT("解锁植物数量"), to_string(_global->userInformation->getUsePlantsNumbers()), Vec2(765, 142), Vec2(886, 142));*/
	onShowTextData(GAME_TEXT("第一次游戏时间"), UserDefault::getInstance()->getStringForKey("FIRSTRUNTIME"), Vec2(242, 74), Vec2(687, 74));
}

void DetailedList::onShowTextData(const string& name, const string& name2, const Vec2& pos, const Vec2& pos2, int size, int size2)
{
	auto str = Text::create();
	str->setString(name);
	str->setFontName(GAME_FONT_NAME_1);
	str->setFontSize(size);
	str->setPosition(pos);
	this->addChild(str);

	auto num = Text::create();
	num->setString(name2);
	num->setFontName(GAME_FONT_NAME_1);
	num->setFontSize(size2);
	num->setPosition(pos2);
	num->setColor(Color3B(0, 255, 255));
	this->addChild(num);
}

void DetailedList::onShowSumList()
{
	auto num = min(_modernPLevel + _modernKLevel + _bigMapPLevel + _bigMapKLevel, 208);
	auto num2 = min(_hammerLevel, 170);
	auto num3 = min(_testGroundLevel, 200);
	auto num4 = min(_izeLevel, 251);
	
	auto sum = num / 208.f;
	auto sum2 = num2 / 170.f;
	auto sum3 = num3 / 200.f;
	auto sum4 = num4 / 251.f;

	auto mGame = sum * 180;
	auto lGame = sum2 * 54;
	auto lGame2 = sum3 * 36;
	auto lGame3 = sum4 * 90;

	auto allSum = to_string(round((sum * 0.5f + sum2 * 0.15f + sum3 * 0.1f + sum4 * 0.25f) * 10000) / 100.f);
	allSum.erase(allSum.find(".") + 3);

	onShowTextData(GAME_TEXT("游戏100%清单"), allSum + "%", Vec2(1435, 1015), Vec2(1435, 520), 50, 130);

	for (float i = 0; i < 360; i+=0.02f)
	{
		auto rad = i * M_PI / 180.f;

		auto pos_O_X = sin(rad) * 300 + 1435;
		auto pos_O_Y = cos(rad) * 300 + 520;

		auto pos_D_X = sin(rad) * 450 + 1435;
		auto pos_D_Y = cos(rad) * 450 + 520;

		if (i <= mGame)
		{
			_drawNode->drawLine(Vec2(pos_O_X, pos_O_Y), Vec2(pos_D_X, pos_D_Y), Color4F(1, 0.5f, 0, 0.5f));
		}
		else if (i > mGame && i <= mGame + lGame)
		{
			_drawNode->drawLine(Vec2(pos_O_X, pos_O_Y), Vec2(pos_D_X, pos_D_Y), Color4F(0, 0, 1, 0.5f));
		}
		else if (i > mGame + lGame && i <= mGame + lGame + lGame2)
		{
			_drawNode->drawLine(Vec2(pos_O_X, pos_O_Y), Vec2(pos_D_X, pos_D_Y), Color4F(1, 1, 0, 0.5f));
		}
		else if (i > mGame + lGame + lGame2 && i <= mGame + lGame + lGame2 + lGame3)
		{
			_drawNode->drawLine(Vec2(pos_O_X, pos_O_Y), Vec2(pos_D_X, pos_D_Y), Color4F(1, 0, 0.5f, 0.5f));
		}
		else
		{
			_drawNode->drawLine(Vec2(pos_O_X, pos_O_Y), Vec2(pos_D_X, pos_D_Y), Color4F(0.5f, 0.5f, 0.5f, 0.4f));
		}
	}
}
