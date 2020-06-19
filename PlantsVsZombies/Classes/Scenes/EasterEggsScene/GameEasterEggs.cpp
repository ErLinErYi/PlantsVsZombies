/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.6.25
 *Email: 2117610943@qq.com
 */

#include "GameEasterEggs.h"

#include "Scenes/MainMenuScene/QuitScene.h"
#include "Based/PlayMusic.h"
#include "Based/VideoPlayer.h"

GameEasterEggs::GameEasterEggs():
	_eggstext(nullptr)
,   _global(Global::getInstance())
,   _audioId(-1)
,   _arcSpeed(CC_DEGREES_TO_RADIANS(20))
,   _radius(500.0f)
,   _accAngle(0.f)
,   _sprite2(nullptr)
,   _swim(nullptr)
,   _hurt(nullptr)
,   _elapseTransTime(0.f)
,   _moveAction(nullptr)
,  _fileName("resources/Animations/Animation3D/tortoise.c3b")
{
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesEnded = CC_CALLBACK_2(GameEasterEggs::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

GameEasterEggs::~GameEasterEggs()
{
	CC_SAFE_RELEASE(_moveAction);
	CC_SAFE_RELEASE(_hurt);
	CC_SAFE_RELEASE(_swim);
}

Scene* GameEasterEggs::createScene()
{
	return GameEasterEggs::create();
}

bool GameEasterEggs::init()
{
	if (!Scene::init())return false;

#if MYRELEASE
	playVideo();
#endif

#ifndef VIDEO_TEST
	addtortoiseRotate();
	addtortoiseStraight();
	createEggText();
	musicCallBack();
	schedule(schedule_selector(GameEasterEggs::rotateUpdate));
	schedule(schedule_selector(GameEasterEggs::straightUpdate));
#else
#   if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	createSlider();
    playVideo();
#   endif
#endif

	return true;
}

void GameEasterEggs::addtortoiseRotate()
{
	auto sprite = Sprite3D::create(_fileName);
	sprite->setScale(0.7f);
	auto s = Director::getInstance()->getWinSize();
	sprite->setPosition(Vec2(s.width / 2.f + _radius * cosf(_accAngle), s.height / 2.f + _radius * sinf(_accAngle)));
	addChild(sprite);
	_sprite = sprite;
	auto animation = Animation3D::create(_fileName);
	if (animation)
	{
		auto animate = Animate3D::create(animation, 0.f, 1.933f);
		sprite->runAction(RepeatForever::create(animate));
		sprite->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(20.f, 0.2f), ScaleTo::create(10.f, 0.7f), nullptr)));
	}
}

void GameEasterEggs::addtortoiseStraight()
{
	auto sprite = Sprite3D::create(_fileName);
	sprite->setScale(0.35f);
	auto s = Director::getInstance()->getWinSize();
	sprite->setPosition(Vec2(s.width * 4.f / 5.f, s.height / 6.f));
	addChild(sprite);
	_sprite2 = sprite;
	auto animation = Animation3D::create(_fileName);
	if (animation)
	{
		auto animate = Animate3D::create(animation, 0.f, 1.933f);
		_swim = RepeatForever::create(animate);
		sprite->runAction(_swim);

		_swim->retain();
		_hurt = Animate3D::create(animation, 1.933f, 2.8f);
		_hurt->retain();
		_state = State::SWIMMING;
	}

	_moveAction = MoveTo::create(4.f, Vec2(s.width / 5.f, s.height / 6.f));
	_moveAction->retain();
	auto seq = Sequence::create(_moveAction, CallFunc::create(CC_CALLBACK_0(GameEasterEggs::reachEndCallBack, this)), nullptr);
	seq->setTag(100);
	sprite->runAction(seq);
	sprite->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(15.f, 0.55f), ScaleTo::create(8.f, 0.2f), nullptr)));
}

void GameEasterEggs::rotateUpdate(float delta)
{
	_accAngle += delta * _arcSpeed;
	const float pi = static_cast<float>(M_PI);
	if (_accAngle >= 2 * pi)
		_accAngle -= 2 * pi;

	auto s = Director::getInstance()->getWinSize();
	_sprite->setPosition(Vec2(s.width / 2.f + _radius * cosf(_accAngle), s.height / 2.f + _radius * sinf(_accAngle)));

	Quaternion quat;
	Quaternion::createFromAxisAngle(Vec3(0.f, 0.f, 1.f), _accAngle - pi * 0.5f, &quat);
	_sprite->setRotationQuat(quat);
}

void GameEasterEggs::straightUpdate(float dt)
{
	if (_state == State::HURT_TO_SWIMMING)
	{
		_elapseTransTime += dt;

		if (_elapseTransTime >= Animate3D::getTransitionTime())
		{
			_sprite2->stopAction(_hurt);
			_state = State::SWIMMING;
		}
	}
	else if (_state == State::SWIMMING_TO_HURT)
	{
		_elapseTransTime += dt;
		if (_elapseTransTime >= Animate3D::getTransitionTime())
		{
			_sprite2->stopAction(_swim);
			_state = State::HURT;
		}
	}
}

void GameEasterEggs::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
	for (auto touch : touches)
	{
		auto location = touch->getLocation();

		if (_sprite2)
		{
			float len = (_sprite2->getPosition() - location).length();
			if (len < 40)
			{
				//hurt the tortoise
				if (_state == State::SWIMMING)
				{
					_elapseTransTime = 0.0f;
					_state = State::SWIMMING_TO_HURT;
					_sprite2->stopAction(_hurt);
					_sprite2->runAction(_hurt);
					auto delay = DelayTime::create(_hurt->getDuration() - Animate3D::getTransitionTime());
					auto seq = Sequence::create(delay, CallFunc::create(CC_CALLBACK_0(GameEasterEggs::renewCallBack, this)), nullptr);
					seq->setTag(101);
					_sprite2->runAction(seq);
				}
				return;
			}
		}
	}
}

void GameEasterEggs::reachEndCallBack()
{
	auto s = Director::getInstance()->getWinSize();
	_sprite2->stopActionByTag(100);
	auto inverse = MoveTo::create(4.f, Vec2(s.width - _sprite2->getPositionX(), s.height / 6.f));
	inverse->retain();
	_moveAction->release();
	_moveAction = inverse;
	auto rot = RotateBy::create(1.f, Vec3(0.f, 180.f, 0.f));
	auto seq = Sequence::create(rot, _moveAction, CallFunc::create(CC_CALLBACK_0(GameEasterEggs::reachEndCallBack, this)), nullptr);
	seq->setTag(100);
	_sprite2->runAction(seq);
}

void GameEasterEggs::renewCallBack()
{
	//rerun swim action
	_sprite2->runAction(_swim);
	_state = State::HURT_TO_SWIMMING;
	_elapseTransTime = 0.0f;
}

void GameEasterEggs::createEggText()
{
	_audioId = PlayMusic::changeBgMusic("ZombiesWinEggs", false);
	AudioEngine::setVolume(_audioId, 1.0f);

	auto sumTime = QuitMenu::getSumRunTime();
	int hour = sumTime / 3600;
	int min = (sumTime - hour * 3600) / 60;
	int sec = sumTime - hour * 3600 - min * 60;

	/* 文字彩蛋 */
	string buff = { "      真棒！！！首先祝贺你通过了此测试版的所有关卡。感谢你能够花费时间来关注此游戏。\
你们的关注是我开发游戏的最大动力。正是因为你们的关注，才使得我有足够的信念使我继续坚持下去，并把游戏做的更好。\n\
      植物大战僵尸这款游戏是我最喜欢的游戏之一，很久以前我就希望自己能够做出这样一款同人游戏，但是那个时候能力有限，\
直到去年暑假（2018年）我开始制作，暑假做了大约一个多月。但是由于能力有限，制作出的游戏根本没有可玩性。最后不得不放弃了制作。\
但是我并没有真正的放弃，时隔一年（2019年）的暑假我从新开始制作植物大战僵尸。经过两三个月的学习与开发，终于制作出了一个具有\
可玩性的测试版。游戏制作是一个困难而且艰辛的过程，期间遇到过无数问题，有过放弃，也有过兴奋。最终我坚持了下来！\n\
      目前有14种僵尸和13种植物，植物和僵尸的动画都是本人做的，由于做动画的能力有限，有些僵尸和植物动画不能实现。\
（使用spine或者DragonBones制作的2D动画）。今后的更新是不定时的，可能会很慢。本人qq：2117610943 QQ群：926707238\n\n" };

	auto Eggstext = Label::createWithTTF("", GAME_FONT_NAME_1, 36);
	Eggstext->setString(buff + "游戏过程中经历 " + std::to_string(_global->userInformation->getBreakThroughNumbers()) +
		" 次闯关失败！\n游戏过程中使用 " + std::to_string(_global->userInformation->getUsePlantsNumbers()) +
		" 株植物进行防御！\n游戏过程中共有 " + std::to_string(_global->userInformation->getKillZombiesNumbers()) + " 个僵尸牺牲了！\n" +
		UserDefault::getInstance()->getStringForKey("FIRSTRUNTIME") + " 是你第一次运行此游戏的时间！\n" +
		"到目前为止你已经游戏了 " + to_string(hour) + "小时 " + to_string(min) + "分钟 " + to_string(sec) + "秒！");
	Eggstext->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, -430));
	Eggstext->setColor(Color3B(0, 255, 255));
	Eggstext->setMaxLineWidth(1700);
	addChild(Eggstext);

	Eggstext->runAction(Sequence::create(MoveTo::create(25.0f, Director::getInstance()->getWinSize() / 2.0f),
		CallFunc::create([=]()
			{
				auto text = Label::createWithTTF("静静的享受音乐播放完毕", GAME_FONT_NAME_1, 36);
				text->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 70));
				text->setColor(Color3B::WHITE);
				text->setName("Text");
				this->addChild(text);

				this->runAction(RepeatForever::create(Sequence::create(
					CallFunc::create([=]()
						{
							static int I = 0;
							if (I % 3 == 0 || I % 5 == 0 || I % 7 == 0)I++;
							else I += 2;
							auto size = Eggstext->getStringLength();
							for (int i = 0; i < size; i++)
							{
								auto letter = Eggstext->getLetter(i);
								if (letter)
								{
									letter->runAction(Sequence::create(DelayTime::create(0.05f * i), 
										Spawn::create(JumpBy::create(0.7f, Vec2(0, 0), 50, 1), RotateBy::create(0.7f, 360), 
											Sequence::create(TintTo::create(0.35f, Color3B::RED), TintTo::create(0.35f, I % 2 == 0 ? Color3B::GREEN : Color3B(0, 255, 255))), nullptr), nullptr));
								}
							}
						}), DelayTime::create(Eggstext->getStringLength() * 0.05f / 3.0f), nullptr)));

				this->runAction(RepeatForever::create(Sequence::create(
					CallFunc::create([=]()
						{
							auto size1 = text->getStringLength();
							for (int i = 0; i < size1; i++)
							{
								auto letter = text->getLetter(i);
								if (letter)
								{
									letter->runAction(Sequence::create(DelayTime::create(0.1f * i), JumpBy::create(0.5f, Vec2(0, 0), 30, 1), nullptr));
								}
							}
						}), DelayTime::create(text->getStringLength() * 0.1f), nullptr)));

			}), nullptr));
}

void GameEasterEggs::musicCallBack()
{
	AudioEngine::setFinishCallback(_audioId, [=](int i, string name)
		{
			if (this->getChildByName("Text"))
				this->getChildByName("Text")->setVisible(false);

			PlayMusic::changeBgMusic("mainmusic", true);
			
			/* 创建按钮 */
			auto button = ui::Button::create("SeedChooser_Button2.png", "SeedChooser_Button2_Glow.png", "", TextureResType::PLIST);
			button->setTitleText(_global->userInformation->getGameText().find("退出")->second);
			button->setTitleColor(Color3B(0, 255, 255));
			button->setTitleFontSize(18);
			button->setScale(2.0f);
			button->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, 70));
			this->addChild(button);
			button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
				{
					switch (type)
					{
					case ui::Widget::TouchEventType::BEGAN:
						PlayMusic::playMusic("tap2");
						break;
					case ui::Widget::TouchEventType::ENDED:
						Director::getInstance()->popScene();
						break;
					}
				});
		});
}

#if MYRELEASE
void GameEasterEggs::playVideo()
{
	_pPlayer = VideoPlayer::instance();
	this->addChild(_pPlayer);

	string str = FileUtils::getInstance()->fullPathForFilename(_global->userInformation->getGameText().find("Video地址")->second);

	auto n = str.find("/");
	while (n != string::npos)
	{
		str.replace(n, 1, "\\");
		n = str.find("/");
	}

	_pPlayer->play(str.c_str());

#if MYRELEASE
#else
	schedule([=](float)
		{
			auto value = _pPlayer->getPrecent();
			_slider->setPercent(value == -1 ? 0 : value);
		}, 1.f, CC_REPEAT_FOREVER, 5.f, "video");
#endif
}
#endif

#ifdef VIDEO_TEST
void GameEasterEggs::createSlider()
{
	_slider = Slider::create();
	_slider->loadBarTexture("bgFile.png", TextureResType::PLIST);
	_slider->loadSlidBallTextureNormal("thumbFile.png", TextureResType::PLIST);
	_slider->loadProgressBarTexture("progressFile.png", TextureResType::PLIST);
	_slider->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.f, 25));
	_slider->setLocalZOrder(1);
	_slider->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
		{
			auto slider = (Slider*)sender;
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				_pPlayer->setPrecent(slider->getPercent() / 100.f);
				break;
			}
		});
	this->addChild(_slider);

	auto KeyBoard = EventListenerKeyboard::create();
	KeyBoard->onKeyReleased = [&](EventKeyboard::KeyCode code, Event* event)
	{
		switch (code)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			_pPlayer->setVideoTime(_pPlayer->getVideoTime() - 30000);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			_pPlayer->setVideoTime(_pPlayer->getVideoTime() + 30000);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
			_pPlayer->setVolume(_pPlayer->getVolume() + 5);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			_pPlayer->setVolume(_pPlayer->getVolume() - 5);
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
			_pPlayer->changeVideoState();
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE:
			Director::getInstance()->popScene();
			break;
		default:
			break;
		}
	};
	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(KeyBoard, this);
}
#endif