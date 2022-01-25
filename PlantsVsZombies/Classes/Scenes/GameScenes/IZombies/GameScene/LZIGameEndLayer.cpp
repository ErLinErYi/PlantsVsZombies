/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.03
 *Email: 2117610943@qq.com
 */

#include "LZIGameEndLayer.h"
#include "LZIControlLayer.h"
#include "LZITemporaryReplaceScene.h"
#include "LZIZombiesData.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZUserData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseQuitLayer.h"
#include "Scenes/MainMenuScene/LZMainMenu.h"

IGameEndLayer::IGameEndLayer()
{
	srand(time(nullptr));
}

IGameEndLayer::~IGameEndLayer()
{
}

bool IGameEndLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createShieldLayer(this);
	_director->getScheduler()->setTimeScale(1.0f);

	return true;
}

void IGameEndLayer::rewardThing()
{
	auto moneybag_hi_res = cocos2d::ui::Button::create("moneybag_hi_res.png", "", "", cocos2d::ui::Widget::TextureResType::PLIST);
	moneybag_hi_res->setPosition(Vec2(700 + rand() % 1000, 100 + rand() % 700));
	moneybag_hi_res->runAction(JumpBy::create(0.5f, Vec2(100 - rand() % 200, 0), 100 + rand() % 100, 1));
	moneybag_hi_res->setName("moneybag_hi_res");
	moneybag_hi_res->setLocalZOrder(2);
	this->addChild(moneybag_hi_res);

	moneybag_hi_res->runAction(RepeatForever::create(Sequence::create(TintTo::create(0.3f, 150, 150, 150), TintTo::create(0.3f, 255, 255, 255), nullptr)));
	moneybag_hi_res->addTouchEventListener([this, moneybag_hi_res](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
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

void IGameEndLayer::rewardCoin(Button* button)
{
	const auto coin = max(UserData::getInstance()->openIntUserData(const_cast<char*>("IZOMBIES_LEVEL_NUMBER")), 1);
	const int number = min(coin / 2, 10) + rand() % coin + 1;

	button->runAction(Sequence::create(Repeat::create(Sequence::create(MoveBy::create(0.05f, Vec2(5, 5)),
		MoveBy::create(0.05f, Vec2(-5, -5)), nullptr), number / 2), DelayTime::create(0.5f), FadeOut::create(0.5f),
		CallFunc::create([button]() {button->removeFromParent(); }), nullptr));

	for (int i = 0; i < number; i++)
	{
		coinAction(button->getPosition() - Vec2(-10, 55), i);

		if (i >= number - 1)
		{
			this->runAction(Sequence::create(
				CallFunc::create([=]()
					{
						auto white = Sprite::createWithSpriteFrameName("White.png");
						white->setAnchorPoint(Vec2(0, 0));
						white->setContentSize(_director->getWinSize());
						white->setOpacity(0);
						white->setGlobalZOrder(20);
						white->setColor(Color3B::BLACK);
						this->addChild(white);
						white->runAction(Sequence::create(DelayTime::create(1.2f + 0.05f * number), FadeIn::create(0.5f),
							CallFunc::create([=]()
								{
									if (IControlLayer::onGetCurrentLevel() == IControlLayer::onGetMostLevel())
									{
										UserData::getInstance()->caveUserData(const_cast<char*>("IZOMBIES_MOST_LEVEL"), static_cast<int>(IControlLayer::onGetMostLevel() + 1));
									}
									UserData::getInstance()->caveUserData(const_cast<char*>("COINNUMBERS"), _global->userInformation->getCoinNumbers());
									UserData::getInstance()->caveUserData(const_cast<char*>("IZOMBIES_LEVEL_NUMBER"), static_cast<int>(IControlLayer::onGetCurrentLevel() + 1));
									UserData::getInstance()->caveUserData(const_cast<char*>("IZOMBIES_SUN_NUMBER"), _global->userInformation->getSunNumbers());
									Director::getInstance()->replaceScene(TransitionFade::create(0.5f, IContinueOrExitLayer::create()));

								}), nullptr));
					}), nullptr));
		}
	}
}

void IGameEndLayer::showFailText()
{
	IControlLayer::beginNewGame();

	auto lose = Sprite::createWithSpriteFrameName("Brain.png");
	lose->setScale(2.0f);
	lose->setPosition(Vec2(2500, Director::getInstance()->getWinSize().height / 2.0f));
	lose->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.05f, Vec2(10, 0)), MoveBy::create(0.05f, Vec2(-10, 0)), nullptr)));
	lose->runAction(Sequence::create(JumpTo::create(1.0f, Director::getInstance()->getWinSize() / 2.0f, 250, 3),
		DelayTime::create(2), CallFunc::create([this]() {Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MainMenu::create())); }), nullptr));
	this->addChild(lose);

	auto text = Text::create(GAME_TEXT("你输了"), GAME_FONT_NAME_1, GAME_TEXT_SIZE("你输了"));
	text->setPosition(Director::getInstance()->getWinSize() / 2.f);
	text->setColor(Color3B::RED);
	text->setOpacity(0);
	text->runAction(FadeIn::create(2.f));
	this->addChild(text);

	IZombiesData::getInstance()->createNewLevelDataDocument();
	IZombiesData::getInstance()->removeLevelData(const_cast<char*>("IZombiesData"));
}
