/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */

#include "LZHGameEndLayer.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZUserData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseQuitLayer.h"
#include "Scenes/MainMenuScene/LZMainMenu.h"

HGameEndLayer::HGameEndLayer()
{
	srand(time(nullptr));
}

HGameEndLayer::~HGameEndLayer()
{
}

bool HGameEndLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

    createShieldLayer(this);

    return true;
}

void HGameEndLayer::successfullEntry()
{
	PlayMusic::playMusic("achievement");
	GSPauseQuitLayer::pauseLayer();
	Director::getInstance()->getOpenGLView()->setCursorVisible(true);
	Director::getInstance()->getOpenGLView()->setCursor("resources/images/System/cursor.png", Point::ANCHOR_TOP_LEFT);
	
	rewardThing();
}

void HGameEndLayer::breakThrough()
{
	PlayMusic::playMusic("losemusic");
	GSPauseQuitLayer::pauseLayer();

	Director::getInstance()->getOpenGLView()->setCursorVisible(true);
	Director::getInstance()->getOpenGLView()->setCursor("resources/images/System/cursor.png", Point::ANCHOR_TOP_LEFT);

	showFailText();
}

void HGameEndLayer::rewardThing()
{
	auto moneybag_hi_res = ui::Button::create(
		UserData::getInstance()->openIntUserData("HAMMERZOMBIES_LEVEL_NUMBER") == 50 ? "CarTrophy.png" : "moneybag_hi_res.png", "", "", TextureResType::PLIST);
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

void HGameEndLayer::rewardCoin(Button* button)
{
	auto coin = UserData::getInstance()->openIntUserData("HAMMERZOMBIES_LEVEL_NUMBER");
	const int number = min(coin / 2, 10) + rand() % (coin / 2);

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
									UserData::getInstance()->caveUserData("COINNUMBERS", _global->userInformation->getCoinNumbers());
									Director::getInstance()->replaceScene(MainMenu::createScene());
								}), nullptr));
					}), nullptr));
		}
	}
}

void HGameEndLayer::showFailText()
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
		DelayTime::create(5), CallFunc::create([this]() {Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MainMenu::createScene())); }), nullptr));
	this->addChild(lose);
}
