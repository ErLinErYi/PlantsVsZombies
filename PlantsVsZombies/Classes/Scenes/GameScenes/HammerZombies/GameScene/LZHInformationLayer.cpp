/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */

#include "LZHInformationLayer.h"
#include "LZHButtonLayer.h"
#include "Based/LZUserData.h"
#include "Based/LZPlayMusic.h"

HInformationLayer::HInformationLayer():
	_informationLayer(nullptr),
	_hammerText(nullptr)
{
	srand(time(nullptr));
}

HInformationLayer::~HInformationLayer()
{
}

bool HInformationLayer::init()
{
	if (!Layer::init())return false;

	showUserText();
	showTipsText();
	showProgressBar();
	showCoinNumbers();
	showZombiesDieNumbers();
	setBeginAnimation();
	
	return true;
}

void HInformationLayer::showUserText()
{
	auto username = ui::Text::create();
	username->setString("“" + _global->userInformation->getUserName() + "”" +
		_global->userInformation->getGameText().find("手指灵活度与忍耐度训练")->second->text +
		_global->userInformation->getGameText().find("锤僵尸小游戏")->second->text +
		StringUtils::format(_global->userInformation->getGameText().find("第 %d 天")->second->text.c_str(),
			max(UserData::getInstance()->openIntUserData("HAMMERZOMBIES_LEVEL_NUMBER"), 1)) +
		_global->userInformation->getGameText().find("无尽模式")->second->text);
	username->setFontName(GAME_FONT_NAME_1);
	username->setFontSize(_global->userInformation->getGameText().find("手指灵活度与忍耐度训练")->second->fontsize);
	username->setColor(Color3B(0, 255, 255));
	username->enableGlow(Color4B::ORANGE);
	username->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 950));
	this->addChild(username);
}

void HInformationLayer::showProgressBar()
{
	auto progressBarBackgroundImage = Sprite::createWithSpriteFrameName("ProgressBar.png");
	progressBarBackgroundImage->setContentSize(Size(465, 40));
	progressBarBackgroundImage->setName("ProgressBar");
	progressBarBackgroundImage->setAnchorPoint(Vec2(1, 0.3f));
	progressBarBackgroundImage->setPosition(Vec2(1300, 1020));
	this->addChild(progressBarBackgroundImage);

	_progressBar = ProgressTimer::create(Sprite::createWithSpriteFrameName("ProgressBar1.png"));
	_progressBar->setScaleX(3.0f);
	_progressBar->setScaleY(2.0f);
	_progressBar->setPosition(Vec2(240, 20));
	_progressBar->setType(ProgressTimer::Type::BAR);  //设置条形进度条
	_progressBar->setBarChangeRate(Vec2(1, 0));       //设置横向
	_progressBar->setMidpoint(Vec2(1, 0));            //设置从左往右
	progressBarBackgroundImage->addChild(_progressBar, -1);

	auto zombiesHead = Sprite::createWithSpriteFrameName("ZombieHead.png");
	zombiesHead->setScale(0.9f);
	zombiesHead->setPosition(Vec2(470, 22));
	zombiesHead->setName("progressbarhead");
	progressBarBackgroundImage->addChild(zombiesHead, 1);

	auto progressbarflag = Sprite::createWithSpriteFrameName("ProgressBarFlag.png");
	progressbarflag->setPosition(Vec2(0, 0));
	progressbarflag->setScale(1.6f);
	progressbarflag->setName("_progressbarflag");
	progressbarflag->setAnchorPoint(Vec2(0, 0));
	progressBarBackgroundImage->addChild(progressbarflag);
}

void HInformationLayer::showTipsText()
{
	auto n = rand() % 2 + 1;
	auto tips = Text::create();
	tips->setFontName(GAME_FONT_NAME_1);
	tips->setString(_global->userInformation->getGameText().find("锤僵尸小提示" + to_string(n))->second->text);
	tips->setFontSize(_global->userInformation->getGameText().find("锤僵尸小提示" + to_string(n))->second->fontsize);
	tips->setColor(Color3B::MAGENTA);
	tips->enableGlow(Color4B::GREEN);
	tips->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 85));
	tips->runAction(Sequence::create(DelayTime::create(60.f), FadeOut::create(1.f), CallFunc::create([tips]() {tips->removeFromParent(); }), nullptr));
	this->addChild(tips);
}

void HInformationLayer::setBeginAnimation()
{
	runAction(Sequence::create(DelayTime::create(1.f), 
		CallFunc::create([=]() 
			{
				runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([]() {PlayMusic::playMusic("readysetplant"); }), nullptr));

				auto text = Sprite::createWithSpriteFrameName("begin.png");
				text->setScale(15.0f);
				text->setPosition(Director::getInstance()->getWinSize() / 2.0f);
				text->runAction(Sequence::create(ScaleTo::create(0.3f, 2.0f), DelayTime::create(0.2f),
					FadeOut::create(0.2f), CallFunc::create([text]() {text->removeFromParent(); }), nullptr));
				this->addChild(text);

				text = Sprite::createWithSpriteFrameName("active.png");
				text->setScale(15.0f);
				text->setOpacity(0);
				text->setPosition(Director::getInstance()->getWinSize() / 2.0f);
				text->runAction(Sequence::create(DelayTime::create(0.5f), FadeIn::create(0.f), ScaleTo::create(0.3f, 2.0f), DelayTime::create(0.2f),
					FadeOut::create(0.2f), CallFunc::create([text]() {text->removeFromParent(); }), nullptr));
				this->addChild(text);

				text = Sprite::createWithSpriteFrameName("finger.png");
				text->setScale(15.0f);
				text->setOpacity(0);
				text->setPosition(Director::getInstance()->getWinSize() / 2.0f);
				text->runAction(Sequence::create(DelayTime::create(1.f), FadeIn::create(0.f), ScaleTo::create(0.3f, 2.0f), DelayTime::create(1.5f),
					FadeOut::create(0.5f), CallFunc::create([text]() {text->removeFromParent(); }), nullptr));
				this->addChild(text);
			}), nullptr));
}

void HInformationLayer::showHammerInformation(Button* button)
{
	if (!_informationLayer)
	{
		_informationLayer = LayerColor::create(Color4B(0, 0, 0, 220));
		_informationLayer->setContentSize(Size(400, 100));
		_informationLayer->setAnchorPoint(Vec2(0, 1));
		this->addChild(_informationLayer, 2);

		auto draw = DrawNode::create();
		draw->setLineWidth(1);
		draw->drawRect(Vec2(0, 0), _informationLayer->getContentSize(), Color4F(0, 1, 1, 1));
		_informationLayer->addChild(draw);
	}
	
	if (!_hammerText)
	{
		_hammerText = Text::create();
		_hammerText->setFontName(GAME_FONT_NAME_1);
		_hammerText->setFontSize(20);
		_hammerText->setTextVerticalAlignment(TextVAlignment::CENTER);
		_hammerText->setTextHorizontalAlignment(TextHAlignment::CENTER);
		_hammerText->setTextAreaSize(_informationLayer->getContentSize());
		_hammerText->setColor(Color3B(0, 255, 255));
		_hammerText->setPosition(_informationLayer->getContentSize() / 2.f);
		_informationLayer->addChild(_hammerText);
	}

	if (!_informationLayer->isVisible())_informationLayer->setVisible(true);

	auto text = _global->userInformation->getGameText();
	auto tag = button->getTag();
	_informationLayer->setPosition(button->getPosition() - Vec2(-80, _informationLayer->getContentSize().height / 2.f));
	_hammerText->setString(text.find("初始攻击力")->second->text + to_string(hammerInformation[tag].lowAttack) + "\n" +
		text.find("最高攻击力")->second->text + to_string(hammerInformation[button->getTag()].lowAttack + hammerInformation[tag].HigtAttack) + "\n" +
		text.find("解锁条件")->second->text + StringUtils::format(_global->userInformation->getGameText().find("解锁该武器")->second->text.c_str(), hammerInformation[tag].unLock));
}

void HInformationLayer::deleteHammerInformation()
{
	if (_informationLayer)
	{
		_informationLayer->setVisible(false);
	}
}

