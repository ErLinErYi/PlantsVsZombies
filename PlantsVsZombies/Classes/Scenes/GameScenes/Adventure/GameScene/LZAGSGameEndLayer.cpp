/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.14
 *Email: 2117610943@qq.com
 */

#include "LZAGSGameEndLayer.h"
#include "LZAGSData.h"
#include "LZAGSInformationLayer.h"
#include "LZAGSPauseQuitLayer.h"
#include "LZAGSGameTimerLayer.h"

#include "Scenes/GameScenes/BigMap/GameScene/LZBigMapGameScene.h"
#include "Scenes/GameScenes/Adventure/WorldScene/LZModernWorld.h"
#include "Scenes/GameScenes/BigMap/WorldScene/LZBigMapWorld.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSControlLayer.h"
#include "Based/LZGlobalVariable.h"
#include "Based/LZUserWinRequirement.h"
#include "Based/LZGameType.h"
#include "Based/LZLevelData.h"
#include "Based/LZCar.h"
#include "Based/LZUserData.h"
#include "Based/LZPlayMusic.h"

GSGameEndLayer::GSGameEndLayer():
	_userWinRequirement(nullptr)
,	_global(Global::getInstance())
,   _openLevelData(OpenLevelData::getInstance())
{
	srand(time(nullptr));
}

GSGameEndLayer::~GSGameEndLayer()
{
	if (_userWinRequirement)delete _userWinRequirement;
}

bool GSGameEndLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0,0,0,180)))return false;

    createShieldLayer(this);

    return true;
}

void GSGameEndLayer::judgeBreakThroughAboutJumpLevel()
{
	// 如果此次闯关时间超过150秒并且闯关失败，则满足一次闯关失败
	if (GSGameTimerLayer::breakThroughTime > 150) 
	{
		UserData::getInstance()->caveUserData(
			Global::getInstance()->userInformation->getCurrentCaveFileLevelWorldName(),
			SPSControlLayer::getLevelBreakThroughNumbers() + 1);
	}
}

void GSGameEndLayer::successfullEntry()
{
	GSPauseQuitLayer::pauseLayer();
	Director::getInstance()->getScheduler()->setTimeScale(1.0f); /* 恢复播放倍数 */
	Director::getInstance()->getOpenGLView()->setCursor("resources/images/System/cursor.png", Point::ANCHOR_TOP_LEFT);
	UserData::getInstance()->createNewLevelDataDocument();
	UserData::getInstance()->removeLevelData(_global->userInformation->getCurrentCaveFileLevelWorldName());

	caveLevelNumber();
	carsToCoins();

	rewardThing();
}

void GSGameEndLayer::breakThrough(GameTypes gameType)
{
	GSPauseQuitLayer::pauseLayer();
	Director::getInstance()->getScheduler()->setTimeScale(1.0f); /* 恢复播放倍数 */
	Director::getInstance()->getOpenGLView()->setCursor("resources/images/System/cursor.png", Point::ANCHOR_TOP_LEFT);
	UserData::getInstance()->caveUserData("BREAKTHROUGH",++_global->userInformation->getBreakThroughNumbers());/* 记录闯关失败个数 */

	UserData::getInstance()->createNewLevelDataDocument();
	UserData::getInstance()->removeLevelData(_global->userInformation->getCurrentCaveFileLevelWorldName());

	PlayMusic::playMusic("losemusic");

	judgeBreakThroughAboutJumpLevel();
	
	if (gameType == GameTypes::UserLose){
		showFailText();
	}
	else{
		showFailDialog(gameType);
	}
}

void GSGameEndLayer::showFailDialog(GameTypes gameType)
{
	_userWinRequirement = new UserWinRequirement(this);
	_userWinRequirement->createDialogBox(gameType);
	_userWinRequirement->setShowDialogAction();

	auto button = Button::create("Continue1.png", "Continue.png", "", TextureResType::PLIST);
	button->setTitleText(Global::getInstance()->userInformation->getGameText().find("继续")->second->text);
	button->setTitleFontName(GAME_FONT_NAME_1);
	button->setTitleFontSize(Global::getInstance()->userInformation->getGameText().find("继续")->second->fontsize);
	button->setTitleColor(Color3B::YELLOW);
	button->setPosition(Vec2(_userWinRequirement->getDialog()->getContentSize().width / 2.0f, 10));
	button->setGlobalZOrder(10);
	button->setScale(0.5f);
	button->runAction(FadeIn::create(0.5f));
	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("gravebutton");
				break;
			case ui::Widget::TouchEventType::ENDED:
				_userWinRequirement->setDelectDialogAction();
				ModernWorld::setPopEnter(true);
				quitScene();
				break;
			}
		});
	_userWinRequirement->getDialog()->addChild(button);
}

void GSGameEndLayer::showFailText()
{
	auto brain = Sprite::createWithSpriteFrameName("Brain.png");
	brain->setPosition(_director->getWinSize() / 2.0f);
	brain->setScale(2.0f);
	brain->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.05f, Vec2(10, 0)), MoveBy::create(0.05f, Vec2(-10, 0)), nullptr)));
	this->addChild(brain);

	auto lose = Sprite::createWithSpriteFrameName(rand() % 2 == 0 ? "zombieswon.png" : "zombieswon1.png");
	lose->setScale(2.0f);
	lose->setPosition(Vec2(2500, Director::getInstance()->getWinSize().height / 2.0f));
	lose->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.05f, Vec2(10, 0)), MoveBy::create(0.05f, Vec2(-10, 0)), nullptr)));
	lose->runAction(Sequence::create(JumpTo::create(1.0f, Director::getInstance()->getWinSize() / 2.0f, 250, 3),
		DelayTime::create(5), CallFunc::create([this]() {	quitScene(); }), nullptr));
	this->addChild(lose);
}

void GSGameEndLayer::rewardThing()
{
	ui::Button* moneybag_hi_res;
	if (_global->userInformation->getCurrentPlayLevels() >= 52 && _global->userInformation->getGameDifficulty())
		moneybag_hi_res = ui::Button::create("trophy.png", "", "", TextureResType::PLIST);
	else if (_global->userInformation->getCurrentPlayLevels() >= 52 && !_global->userInformation->getGameDifficulty())
		moneybag_hi_res = ui::Button::create("trophy1.png", "", "", TextureResType::PLIST);
	else
		moneybag_hi_res = ui::Button::create("moneybag_hi_res.png", "", "", TextureResType::PLIST);

	moneybag_hi_res->setPosition(Vec2(700 + rand() % 1000, 100 + rand() % 700));
	moneybag_hi_res->runAction(JumpBy::create(0.5f, Vec2(100 - rand() % 200, 0), 100 + rand() % 100, 1));
	moneybag_hi_res->setName("moneybag_hi_res");
	moneybag_hi_res->setLocalZOrder(2);
	this->addChild(moneybag_hi_res);

	moneybag_hi_res->runAction(RepeatForever::create(Sequence::create(TintTo::create(0.3f, 150, 150, 150), TintTo::create(0.3f, 255, 255, 255), nullptr)));
	moneybag_hi_res->addTouchEventListener([this, moneybag_hi_res](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("coin");
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				moneybag_hi_res->setEnabled(false);
				moneybag_hi_res->setBright(true);

				PlayMusic::playMusic("moneyfalls");
				rewardCoin(moneybag_hi_res);
				break;
			}
		});
}

void GSGameEndLayer::caveLevelNumber()
{
	/* 记录关数 */
	if (_global->userInformation->getUserSelectWorldData().at(
		_global->userInformation->getCurrentPlayWorldTag())->levels == _global->userInformation->getCurrentPlayLevels())
	{
		string worldFile;
		if (_global->userInformation->getGameDifficulty())
		{
			worldFile = StringUtils::format(_global->userInformation->getSystemDifCaveFileName().c_str(), _global->userInformation->getCurrentPlayWorldTag() + 1);
		}
		else
		{
			worldFile = StringUtils::format(_global->userInformation->getSystemCaveFileName().c_str(), _global->userInformation->getCurrentPlayWorldTag() + 1);
		}
		
		UserData::getInstance()->caveUserData(const_cast<char*>(worldFile.c_str()),
			++_global->userInformation->getUserSelectWorldData().at(_global->userInformation->getCurrentPlayWorldTag())->levels);
	}

	if (_global->userInformation->getCurrentPlayLevels() >= 52)
	{
		UserData::getInstance()->caveUserData("ISBEGINSHOWEGGS", true);
		_global->userInformation->setIsShowEggs(true);
	}
}

void GSGameEndLayer::carsToCoins()
{
	int i = -1;
	for (auto car : CarsGroup)
	{
		if (!car->getLive())
		{
			auto cars = car;
			runAction(Sequence::create(DelayTime::create(0.4f * ++i), CallFunc::create([this, cars]()
				{
					PlayMusic::playMusic("coin");
					cars->getCar()->runAction(Sequence::create( 
						Spawn::create(ScaleTo::create(0.2f, 0), FadeOut::create(0.2f), nullptr), nullptr));
					cars->getCar()->setVisible(false);

					auto pos = cars->getCar()->getPosition();
					if (BigMapGameScene::scrollView)
					{
						auto offset = BigMapGameScene::scrollView->getContentOffset();
						pos = Vec2(pos.x - fabs(offset.x), pos.y - fabs(offset.y));
					}
					coinAction(pos, 0, true);
				}), nullptr));
		}
	}
}

void GSGameEndLayer::rewardCoin(Button* button)
{
	const int coins = _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getCoinNumbers() / 2;
	const int number = coins + rand() % coins;

	button->runAction(Sequence::create(Repeat::create(Sequence::create(MoveBy::create(0.05f, Vec2(5, 5)),
		MoveBy::create(0.05f, Vec2(-5, -5)), nullptr), number / 2), DelayTime::create(0.5f), FadeOut::create(0.5f),
		CallFunc::create([button]() {button->removeFromParent(); }), nullptr));

	for (int i = 0; i < number; i++)
	{
		coinAction(button->getPosition() - Vec2(-10, 55), i);

		if (i >= number - 1)
		{
			this->runAction(Sequence::create(DelayTime::create(2.0f),
				CallFunc::create([=]()
					{
						auto audio = PlayMusic::playMusic("winmusic", 0);
						PlayMusic::setMusicVolume(audio);
						AudioEngine::setFinishCallback(audio, [=](int i, string name)
							{
								PlayMusic::playMusic("lightfill");
							});

						auto AwardRays = Sprite::createWithSpriteFrameName("AwardRays.png");
						AwardRays->setPosition(_director->getWinSize() / 2.0f);
						AwardRays->runAction(RepeatForever::create(Spawn::create(RotateBy::create(0.5f, 30), ScaleBy::create(0.5f, 1.5f), nullptr)));
						AwardRays->setGlobalZOrder(20);
						this->addChild(AwardRays);

						auto White = Sprite::createWithSpriteFrameName("White.png");
						White->setAnchorPoint(Vec2(0, 0));
						White->setContentSize(_director->getWinSize());
						White->setOpacity(0);
						White->setGlobalZOrder(20);
						this->addChild(White);
						White->runAction(Sequence::create(DelayTime::create(1.0f), FadeIn::create(7.0f),
							CallFunc::create([=]()
								{
									/* 保存金币数 */
									UserData::getInstance()->caveUserData("COINNUMBERS", _global->userInformation->getCoinNumbers());
									
									ModernWorld::setPopEnter(true);
									_director->popScene();
								}), nullptr));
					}), nullptr));
		}
	}
}

void GSGameEndLayer::coinAction(const Vec2& position, const int id, const bool big)
{
	auto coin = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("coin")->second);
	coin->setPosition(position);
	coin->setScale(0.05f);
	coin->update(0);
	this->addChild(coin);

	auto callFunc = CallFunc::create([=]()
		{
			PlayMusic::playMusic("moneyfalls");
			coin->removeFromParent();
			big ? _global->userInformation->setCoinNumbers(_global->userInformation->getCoinNumbers() + 10) :
				_global->userInformation->setCoinNumbers(_global->userInformation->getCoinNumbers() + 1);
			informationLayerInformation->updateCoinNumbers();
		});
	
	auto action = Spawn::create(FadeIn::create(0.5f), ScaleTo::create(0.5f, 0.8f), 
		JumpBy::create(0.5f, Vec2(150 - rand() % 300, 100 - rand() % 100), rand() % 100 + 200, 1),
		Sequence::create(DelayTime::create(0.25f), CallFunc::create([coin]() {coin->setLocalZOrder(3); }), nullptr), nullptr);
	auto action1 = Spawn::create(ScaleTo::create(0.2f, 0.5f), FadeOut::create(0.2f), callFunc, nullptr);

	coin->runAction(Sequence::create(DelayTime::create(id * 0.05f), action, DelayTime::create(0.05f), MoveTo::create(0.5f, Vec2(1655, 38)), action1, nullptr));
}

void GSGameEndLayer::quitScene()
{
	auto layer = LayerColor::create(Color4B(0, 0, 0, 0));
	this->addChild(layer);
	layer->runAction(Sequence::create(FadeIn::create(1.0f),
		CallFunc::create([=]()
			{
				ModernWorld::setPopEnter(true);
				layer->removeFromParent();
				_director->popScene();

				GSPauseQuitLayer::resumeLayer();
			}), nullptr));
}
