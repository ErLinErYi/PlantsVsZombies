/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#include "LZTGInformationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSControlLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSZombiesAppearControl.h"
#include "Based/LZUserData.h"

TGInformationLayer::TGInformationLayer():
	_userName(nullptr)
{
	_text = "“" + _global->userInformation->getUserName() + "”" + GAME_TEXT("植物试炼场") + " - ";
	onSetMostFrenquence(UserData::getInstance()->openIntUserData(const_cast<char*>("TESTINGGROUND")));
}

TGInformationLayer::~TGInformationLayer()
{
}

bool TGInformationLayer::init()
{
	if (!Layer::init())return false;

	showUserText();
	showSunNumbers();
	showGameType();

	return true;
}

void TGInformationLayer::showUserText()
{
	_userName = ui::Text::create();
	_userName->setString(_text + StringUtils::format(GAME_TEXT("试炼场记录").c_str(), 1, onGetMostFrenquence() > 1 ? onGetMostFrenquence() : 1));
	_userName->setFontName(GAME_FONT_NAME_1);
	_userName->setFontSize(GAME_TEXT_SIZE("植物试炼场"));
	_userName->setColor(Color3B(0, 255, 255));
	_userName->enableGlow(Color4B::RED);
	_userName->setName("userName");
	_userName->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 50));
	this->addChild(_userName);
}

void TGInformationLayer::showSunNumbers()
{
	auto sunBank = Sprite::createWithSpriteFrameName("SunBank.png");
	sunBank->setScale(0.8f);
	sunBank->setAnchorPoint(Vec2(0, 1));
	sunBank->setName("SunBank");
	sunBank->setPosition(Vec2(235, 1080));
	this->addChild(sunBank);

	_sunNumbersText = ui::Text::create(to_string(_global->userInformation->getSunNumbers()), GAME_FONT_NAME_2, 45);
	_sunNumbersText->setTextAreaSize(Size(170, sunBank->getContentSize().height - 50));
	_sunNumbersText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_sunNumbersText->setTextVerticalAlignment(TextVAlignment::CENTER);
	_sunNumbersText->setPosition(Vec2(215, 65));
	_sunNumbersText->setAnchorPoint(Vec2(0.5f, 0.5f));
	_sunNumbersText->setColor(Color3B(255, 127, 39));
	_sunNumbersText->enableGlow(Color4B(0, 255, 255, 255));
	_sunNumbersText->setOverflow(Overflow::SHRINK);
	_sunNumbersText->setName("SunNumbersText");
	sunBank->addChild(_sunNumbersText);
}

void TGInformationLayer::showGameType()
{
	gameType = new GameType(this);
}

void TGInformationLayer::onSetMostFrenquence(int frequence)
{
	_mostFrenquence = frequence;

	_encryptKey = rand();
	_mostFrenquence ^= _encryptKey;
}

int TGInformationLayer::onGetMostFrenquence()
{
	return _mostFrenquence ^ _encryptKey;
}

void TGInformationLayer::updateText()
{
	if (_userName)
	{
		const auto number = controlLayerInformation->_zombiesAppearControl->getZombiesAppearFrequency();
		if (number > onGetMostFrenquence())onSetMostFrenquence(number);
		_userName->setString(_text + StringUtils::format(GAME_TEXT("试炼场记录").c_str(), number, onGetMostFrenquence()));
		UserData::getInstance()->caveUserData(const_cast<char*>("TESTINGGROUND"), onGetMostFrenquence());
	}
}
