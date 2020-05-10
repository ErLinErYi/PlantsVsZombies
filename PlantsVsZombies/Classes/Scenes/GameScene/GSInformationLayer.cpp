/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#include "GSInformationLayer.h"
#include "Based/GameType.h"
#include "Based/PlayMusic.h"

GSInformationLayer::GSInformationLayer():
	_levelLastPrecent(0.0f)
,	_zombiesDieText(nullptr)
,	_coinNumbersText(nullptr)
,	_sunNumbersText(nullptr)
,	_progressBar(nullptr)
,	_global(Global::getInstance())
,	_openLevelData(OpenLevelData::getInstance())
{
}

GSInformationLayer::~GSInformationLayer()
{
}

bool GSInformationLayer::init()
{
	if(!Layer::init())return false;

	showUserText();
	showProgressBar();
	showZombiesDieNumbers();
	showCoinNumbers();
	showSunNumbers();

	return true;
}

void GSInformationLayer::showUserText()
{
	char buff[128];
	snprintf(buff, 128, "第 %d 天", _global->userInformation->getCurrentPlayLevels());

	auto username = ui::Text::create();
	username->setString("“" + _global->userInformation->getUserName() + "”" + "的时空冒险之旅" + _global->userInformation->getCurrentPlayWorldName() + buff);
	username->setFontName(GAME_FONT_NAME_1);
	username->setFontSize(30);
	username->setColor(Color3B(0, 255, 255));
	username->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 850));
	this->addChild(username);
}

void GSInformationLayer::showProgressBar()
{
	auto progressBarBackgroundImage = Sprite::createWithSpriteFrameName("ProgressBar.png");
	progressBarBackgroundImage->setContentSize(Size(465, 40));
	progressBarBackgroundImage->setName("ProgressBar");
	progressBarBackgroundImage->setAnchorPoint(Vec2(1, 0.3f));
	progressBarBackgroundImage->setPosition(Vec2(1300, 1005));
	this->addChild(progressBarBackgroundImage);

	_progressBar = ProgressTimer::create(Sprite::createWithSpriteFrameName("ProgressBar1.png"));
	_progressBar->setScaleX(3.0f);
	_progressBar->setScaleY(2.0f);
	_progressBar->setPosition(Vec2(240, 20));
	_progressBar->setType(ProgressTimer::Type::BAR);  //设置条形进度条
	_progressBar->setBarChangeRate(Vec2(1, 0));       //设置横向
	_progressBar->setMidpoint(Vec2(1, 0));            //设置从左往右
	//progressbar->setDirection(LoadingBar::Direction::RIGHT);
	progressBarBackgroundImage->addChild(_progressBar, -1);

	auto zombiesHead = Sprite::createWithSpriteFrameName("ZombieHead.png");
	zombiesHead->setScale(0.9f);
	zombiesHead->setPosition(Vec2(470, 22));
	zombiesHead->setName("progressbarhead");
	progressBarBackgroundImage->addChild(zombiesHead, 1);

	auto& leveldata = _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getMunchZombiesFrequency();
	for (unsigned int i = 0; i <leveldata.size(); ++i)
	{
		auto progressbarflag = Sprite::createWithSpriteFrameName("ProgressBarFlag.png");
		progressbarflag->setPosition(Vec2(465 - 465 * (_openLevelData->readLevelData(
			_openLevelData->getLevelNumber())->getMunchZombiesFrequency().at(i) / (double)_openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesFrequency()), 0));
		progressbarflag->setScale(1.6f);
		progressbarflag->setAnchorPoint(Vec2(0, 0));
		progressbarflag->setTag(leveldata.at(i));
		progressBarBackgroundImage->addChild(progressbarflag);
	}
	auto progressbarflag = Sprite::createWithSpriteFrameName("ProgressBarFlag.png");
	progressbarflag->setPosition(Vec2(0, 0));
	progressbarflag->setScale(1.6f);
	progressbarflag->setName("_progressbarflag");
	progressbarflag->setAnchorPoint(Vec2(0, 0));
	progressBarBackgroundImage->addChild(progressbarflag);
}

void GSInformationLayer::showZombiesDieNumbers()
{
	auto ZombiesDie = Sprite::createWithSpriteFrameName("ZombiesDie.png");
	ZombiesDie->setPosition(Vec2(1500, 55));
	this->addChild(ZombiesDie);

	_zombiesDieText = Text::create();
	_zombiesDieText->setPosition(Vec2(130, 30));
	_zombiesDieText->setFontName(GAME_FONT_NAME_1);
	_zombiesDieText->setFontSize(30);
	_zombiesDieText->setColor(Color3B(190, 0, 190));
	ZombiesDie->addChild(_zombiesDieText);

	updateZombiesDieNumbers();
}

void GSInformationLayer::updateZombiesDieNumbers()
{
	_zombiesDieText->setString(to_string(_global->userInformation->getKillZombiesNumbers()));
}

void GSInformationLayer::showCoinNumbers()
{
	auto coinBank = Sprite::createWithSpriteFrameName("coinbank.png");
	coinBank->setScale(0.8f);
	coinBank->setPosition(Vec2(1750, 50));
	this->addChild(coinBank);

	_coinNumbersText = ui::Text::create();
	_coinNumbersText->setFontSize(40);
	_coinNumbersText->setFontName(GAME_FONT_NAME_2);
	_coinNumbersText->setPosition(Vec2(180, 40));
	_coinNumbersText->setAnchorPoint(Vec2(0.5f, 0.5f));
	_coinNumbersText->setColor(Color3B::YELLOW);
	coinBank->addChild(_coinNumbersText);

	updateCoinNumbers();
}

void GSInformationLayer::updateCoinNumbers()
{
	_coinNumbersText->setString(to_string(_global->userInformation->getCoinNumbers()));
}

void GSInformationLayer::showSunNumbers()
{
	auto sunBank = Sprite::createWithSpriteFrameName("SunBank.png");
	sunBank->setScale(0.8f);
	sunBank->setAnchorPoint(Vec2(0, 1));
	sunBank->setName("SunBank");
	sunBank->setPosition(Vec2(235, 1065));
	this->addChild(sunBank);

	_sunNumbersText = ui::Text::create();
	_sunNumbersText->setFontSize(50);
	_sunNumbersText->setFontName(GAME_FONT_NAME_2);
	_sunNumbersText->setPosition(Vec2(215, 70));
	_sunNumbersText->setAnchorPoint(Vec2(0.5f, 0.5f));
	_sunNumbersText->setColor(Color3B(255, 127, 39));
	_sunNumbersText->setName("SunNumbersText");
	sunBank->addChild(_sunNumbersText);

	updateSunNumbers();
}

void GSInformationLayer::showPromptMuchZombiesText(const string& textName)
{
	PlayMusic::playMusic("finalwave");
	
	auto zombiescoming = Sprite::createWithSpriteFrameName(textName + ".png");
	zombiescoming->setPosition(_director->getWinSize() / 2);
	zombiescoming->setScale(5.0f);
	zombiescoming->runAction(Sequence::create(ScaleTo::create(0.5f, 1.0f), DelayTime::create(1.5f),
		CallFunc::create([=]()
			{
				zombiescoming->removeFromParent();
			}), nullptr));
	this->addChild(zombiescoming);
}

void GSInformationLayer::createPromptText()
{
	auto promptText = Sprite::createWithSpriteFrameName("PromptText.png");
	promptText->setColor(Color3B::BLACK);
	promptText->setOpacity(150);
	promptText->setScaleX(1.3f);
	promptText->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 300));
	promptText->setName("PromptText");
	this->addChild(promptText);

	auto requiretext = Label::createWithTTF(_global->userInformation->getGameText().find("提示信息")->second, GAME_FONT_NAME_1, 40);
	requiretext->setColor(Color3B::WHITE);
	requiretext->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 300));
	requiretext->setName("requiretext");
	this->addChild(requiretext);

	promptText->runAction(Sequence::create(DelayTime::create(4.0f), FadeOut::create(1.0f), nullptr));
	requiretext->runAction(Sequence::create(DelayTime::create(4.0f), FadeOut::create(1.0f),
		CallFunc::create([=]()
			{
				this->removeChildByName("PromptText");
				this->removeChildByName("requiretext");
			}), nullptr));
}

void GSInformationLayer::setProgressBarPercent(const float precent)
{
	_progressBar->setPercentage(precent);
}

float GSInformationLayer::getProgressBarPercent() const
{
	return _progressBar->getPercentage();
}

void GSInformationLayer::setProgressBarLastPercent(const float precent)
{
	_levelLastPrecent = precent;
}

float GSInformationLayer::getProgressBarLastPercent() const
{
	return _levelLastPrecent;
}

void GSInformationLayer::updateSunNumbers()
{
	auto sun = Sprite::createWithSpriteFrameName("Sun.png");
	sun->setPosition(Vec2(290, 1010));
	sun->setScale(0.9f);
	this->addChild(sun);
	sun->runAction(Sequence::create(DelayTime::create(0.15f), CallFunc::create([sun]() {sun->removeFromParent(); }), nullptr));

	_sunNumbersText->setString(to_string(_global->userInformation->getSunNumbers()));
}

void GSInformationLayer::updateProgressBar(const int& zombiesAppearFrequency)
{
	float percent = zombiesAppearFrequency / static_cast<float>(_openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesFrequency());
	_progressBar->runAction(ProgressFromTo::create(10.0f, _levelLastPrecent * 100, percent * 100));
	_levelLastPrecent = percent;
}

void GSInformationLayer::updateProgressBar(const int& zombiesAppearFrequency, const int& maxFrequency)
{
	float percent = zombiesAppearFrequency / static_cast<float>(maxFrequency);
	_progressBar->runAction(ProgressFromTo::create(10.0f, _levelLastPrecent * 100, percent * 100));
	_levelLastPrecent = percent;
}

bool GSInformationLayer::updateProgressBarFlag()
{
	if (_progressBar->getPercentage() >= _levelLastPrecent * 100 - 8)
	{
		auto flag = dynamic_cast<Sprite*>(_progressBar->getParent()->getChildByName("_progressbarflag"));
		flag->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ProgressBarFlag1.png"));
		showPromptMuchZombiesText("Final");
		return true;
	}
	return false;
}

bool GSInformationLayer::updateProgressBarFlag(const int& id)
{
	if (_progressBar->getPercentage() >= _levelLastPrecent * 100 - 8)
	{
		if (id == -1)
		{
			showPromptMuchZombiesText("ManyZombiesComing");
		}
		else
		{
			auto flag = dynamic_cast<Sprite*>(_progressBar->getParent()->getChildByTag(id));
			flag->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ProgressBarFlag1.png"));
		}
		return true;
	}
	return false;
}

void GSInformationLayer::updateProgressBarFinalFlag()
{
	auto flag = dynamic_cast<Sprite*>(_progressBar->getParent()->getChildByName("_progressbarflag"));
	flag->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ProgressBarFlag1.png"));
}

void GSInformationLayer::updateProgressBarHead()
{
	_progressBar->getParent()->getChildByName("progressbarhead")->setPosition(Vec2(470 - 460 * (_progressBar->getPercentage() / 100.0f), 22));
}

void GSInformationLayer::sunNumberTextWarning()
{
	auto action = TintBy::create(0.1f, 255, 0, 0);
	_sunNumbersText->runAction(Repeat::create(Sequence::create(action, action->reverse(), CallFunc::create([=]()
		{
			_sunNumbersText->setColor(Color3B(255, 127, 39));
		}), nullptr), 5));
}
