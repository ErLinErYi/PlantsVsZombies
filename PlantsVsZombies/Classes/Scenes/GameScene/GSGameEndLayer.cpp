/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.14
 *Email: 2117610943@qq.com
 */

#include "GSGameEndLayer.h"
#include "GSData.h"
#include "GSInformationLayer.h"
#include "GSPauseQuitLayer.h"

#include "Scenes/WorldScene/World_1.h"
#include "Based/GlobalVariable.h"
#include "Based/UserWinRequirement.h"
#include "Based/GameType.h"
#include "Based/LevelData.h"
#include "Based/Car.h"

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
    if (!Layer::init())return false;

    createShieldLayer();

    return true;
}

void GSGameEndLayer::createShieldLayer()
{
    auto shieldListener = EventListenerTouchOneByOne::create();
    shieldListener->onTouchBegan = [](Touch* touch, Event* event)-> bool { return true; };
    shieldListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(shieldListener, this);
}

void GSGameEndLayer::successfullEntry()
{
	caveLevelNumber();
	carsTocCoins();

	auto moneybag_hi_res = ui::Button::create(_global->userInformation->getImagePath().find("moneybag_hi_res")->second);
	moneybag_hi_res->setPosition(Vec2(700 + rand() % 1000, 100 + rand() % 700));
	moneybag_hi_res->runAction(JumpBy::create(0.5f, Vec2(100 - rand() % 200, 0), 100 + rand() % 100, 1));
	moneybag_hi_res->setGlobalZOrder(20);
	moneybag_hi_res->setName("moneybag_hi_res");
	this->addChild(moneybag_hi_res);

	moneybag_hi_res->runAction(RepeatForever::create(Sequence::create(TintTo::create(0.3f, 150, 150, 150), TintTo::create(0.3f, 255, 255, 255), nullptr)));
	moneybag_hi_res->addTouchEventListener([this, moneybag_hi_res](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("moneyfalls")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				moneybag_hi_res->setEnabled(false);
				moneybag_hi_res->setBright(true);

				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("moneyfalls")->second), _global->userInformation->getSoundEffectVolume());
				rewardCoin(moneybag_hi_res);
				break;
			}
		});
}

void GSGameEndLayer::breakThrough(GameTypes gameType)
{
	GSPauseQuitLayer::pauseLayer();
	_director->getScheduler()->setTimeScale(1.0f);

	/* 记录闯关失败个数 */
	UserDefault::getInstance()->setIntegerForKey("BREAKTHROUGH", _global->userInformation->getBreakThroughnumbers());
	UserDefault::getInstance()->flush();

	AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("losemusic")->second), _global->userInformation->getSoundEffectVolume());

	if (gameType == GameTypes::UserLose)
	{
		showFailText();
	}
	else
	{
		showFailDialog(gameType);
	}
}

void GSGameEndLayer::showFailDialog(GameTypes gameType)
{
	_userWinRequirement = new UserWinRequirement(this);
	_userWinRequirement->createDialogBox(gameType);
	_userWinRequirement->setShowDialogAction();

	auto button = Button::create(Global::getInstance()->userInformation->getImagePath().find("Continue1")->second, Global::getInstance()->userInformation->getImagePath().find("Continue")->second);
	button->setTitleText(Global::getInstance()->userInformation->getGameText().find("继续")->second);
	button->setTitleFontName("resources/fonts/fzse_gbk.ttf");
	button->setTitleFontSize(30);
	button->setTitleColor(Color3B::YELLOW);
	button->setPosition(Vec2(_userWinRequirement->getDialog()->getContentSize().width / 2.0f, 20));
	button->setGlobalZOrder(10);
	button->runAction(FadeIn::create(0.5f));
	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(Global::getInstance()->userInformation->getMusicPath().find("gravebutton")->second), Global::getInstance()->userInformation->getSoundEffectVolume());
				break;
			case ui::Widget::TouchEventType::ENDED:
				_userWinRequirement->setDelectDialogAction();
				_director->replaceScene(TransitionFade::create(1.0f, World_1::createScene()));
				break;
			}
		});
	_userWinRequirement->getDialog()->addChild(button);
}

void GSGameEndLayer::showFailText()
{
	auto layer = LayerColor::create(Color4B(0, 0, 0, 180));
	auto brain = Sprite::create(_global->userInformation->getImagePath().find("Brain")->second);
	brain->setPosition(_director->getWinSize() / 2.0f);
	brain->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.05f, Vec2(10, 0)), MoveBy::create(0.05f, Vec2(-10, 0)), nullptr)));
	layer->addChild(brain);

	auto lose = Sprite::create(_global->userInformation->getImagePath().find(rand() % 2 == 0 ? "zombieswon" : "zombieswon1")->second);
	lose->setScale(2.0f);
	lose->setPosition(Vec2(2500, Director::getInstance()->getWinSize().height / 2.0f));
	lose->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.05f, Vec2(10, 0)), MoveBy::create(0.05f, Vec2(-10, 0)), nullptr)));
	lose->runAction(Sequence::create(JumpTo::create(1.0f, Director::getInstance()->getWinSize() / 2.0f, 250, 3),
		DelayTime::create(5),CallFunc::create([this]()
			{
				_director->replaceScene(TransitionFade::create(1.0f, World_1::createScene()));
			}),
		nullptr));
	layer->addChild(lose);
	this->addChild(layer);
}

void GSGameEndLayer::caveLevelNumber()
{
	/* 记录关数 */
	if (_global->userInformation->getUserSelectWorldData().at(_global->userInformation->getCurrentPlayWorldTag())->levels == _global->userInformation->getCurrentPlayLevels())
	{
		UserDefault::getInstance()->setIntegerForKey(
			_global->userInformation->getGameDifficulty() ?
			_global->userInformation->getSystemDifCaveFileName(_global->userInformation->getUserCaveFileNumber()).c_str() :
			_global->userInformation->getSystemCaveFileName(_global->userInformation->getUserCaveFileNumber()).c_str(),
			++_global->userInformation->getUserSelectWorldData().at(0)->levels);
		UserDefault::getInstance()->flush();
	}
}

void GSGameEndLayer::carsTocCoins()
{
	int i = -1;
	for (auto car : CarsGroup)
	{
		if (!car->getLive())
		{
			auto cars = car;
			runAction(Sequence::create(DelayTime::create(0.2f * ++i), CallFunc::create([this, cars]()
				{
					AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("plastichit2")->second), _global->userInformation->getSoundEffectVolume());
					cars->getCar()->runAction(Sequence::create( Spawn::create(ScaleTo::create(0.2f, 0), FadeOut::create(0.2f), nullptr), nullptr));
					cars->getCar()->setVisible(false);
					coinAction(cars->getCar()->getPosition(), 0);
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
		CallFunc::create([button]()
			{
				button->removeFromParent();
			}),
		nullptr));

	for (int i = 0; i < number; i++)
	{
		coinAction(button->getPosition() - Vec2(-10, 55), i);

		if (i >= number - 1)
		{
			this->runAction(Sequence::create(DelayTime::create(2.0f),
				CallFunc::create([=]()
					{
						auto audio = AudioEngine::play2d(_global->userInformation->getMusicPath().find("winmusic")->second);
						AudioEngine::setVolume(audio, _global->userInformation->getSoundEffectVolume());
						AudioEngine::setFinishCallback(audio, [=](int i, string name)
							{
								AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("lightfill")->second), _global->userInformation->getSoundEffectVolume());
							});

						auto AwardRays = Sprite::create(_global->userInformation->getImagePath().find("AwardRays")->second);
						AwardRays->setPosition(_director->getWinSize() / 2.0f);
						AwardRays->runAction(RepeatForever::create(Spawn::create(RotateBy::create(0.5f, 30), ScaleBy::create(0.5f, 1.5f), nullptr)));
						AwardRays->setGlobalZOrder(20);
						this->addChild(AwardRays);

						auto White = Sprite::create(_global->userInformation->getImagePath().find("White")->second);
						White->setAnchorPoint(Vec2(0, 0));
						White->setContentSize(_director->getWinSize());
						White->setOpacity(0);
						White->setGlobalZOrder(20);
						this->addChild(White);
						White->runAction(Sequence::create(DelayTime::create(1.0f), FadeIn::create(7.0f),
							CallFunc::create([=]()
								{
									/* 保存金币数 */
									UserDefault::getInstance()->setIntegerForKey("COINNUMBERS", _global->userInformation->getCoinNumbers());
									UserDefault::getInstance()->flush();
									_director->replaceScene(TransitionFade::create(1.0f, World_1::createScene()));
								}), nullptr));
					}), nullptr));
		}
	}
}

void GSGameEndLayer::coinAction(const Vec2& position, const int id)
{
	auto coin = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("coin")->second);
	coin->setScale(0.05f);
	coin->setPosition(position);
	coin->setGlobalZOrder(20);
	coin->setOpacity(0);
	coin->setName("coin");
	this->addChild(coin);

	coin->runAction(Sequence::create(DelayTime::create(id * 0.05f),Spawn::create(FadeIn::create(0.5f),ScaleTo::create(0.5f, 0.3f),
		JumpBy::create(0.5f, Vec2(150 - rand() % 300, 100 - rand() % 100), rand() % 100 + 200, 1), nullptr),
		DelayTime::create(0.05f),MoveTo::create(0.5f, Vec2(1650, -40)),Spawn::create(ScaleTo::create(0.2f, 0.5f), FadeOut::create(0.2f),
			CallFunc::create([&]()
				{
					AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("moneyfalls")->second), _global->userInformation->getSoundEffectVolume());
					this->removeChildByName("coin");
					_global->userInformation->setCoinNumbers(_global->userInformation->getCoinNumbers() + 2);
					informationLayerInformation->updateCoinNumbers();
				}), nullptr),
		nullptr));
}
