/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.16
 *Email: 2117610943@qq.com
 */

#include "MainMenu.h"
#include "InputDataScene.h"
#include "Scenes/HelpScene/HelpScene.h"
#include "QuitScene.h"
#include "OptionsSence.h"
#include "Scenes/WorldScene/SelectWorldScene.h"

MainMenu::MainMenu():
	_global(Global::getInstance()),
	_director(Director::getInstance()),
	_mouse(nullptr),
	_userNameActionParent(nullptr),
	_userText(nullptr),
	_inputLayer(nullptr),
	_quitLayer(nullptr),
	_optionLayer(nullptr)
{
}

MainMenu::~MainMenu()
{
}

Scene* MainMenu::createScene()
{
	return MainMenu::create();
}

bool MainMenu::init()
{
	if (!Scene::init())return false;

	/* 播放音乐 */
	_global->changeBgMusic("mainmusic", true);

	AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("roll_in")->second), _global->userInformation->getSoundEffectVolume());
	
	/* 创建主要精灵 */
	this->createMainSprite();

	/* 创建花朵 */
	this->createFlowers();

	/* 创建动画 */
	this->createAnimation();

	/* 创建鼠标监听 */
	this->createMouseListener();

	/* 创建按钮 */
	this->createMainButton();

	/* 定时器 */
	this->schedule(schedule_selector(MainMenu::curUpdate), 0.05f);

	return true;
}

void MainMenu::curUpdate(float time)
{
	/* 鼠标移动到按钮上播放音乐 */
	this->playMusicBleepInMainBuutons(0, Vec2(_cur.x - 606, _cur.y));
	this->playMusicBleepInMainBuutons(1, Vec2(_cur.x - 606, _cur.y));
	this->playMusicBleepInMainBuutons(2, Vec2(_cur.x - 606, _cur.y));
	this->playMusicBleepInMainBuutons(3, _cur);
	
	this->playMusicBleepInGameButtons(1);
	this->playMusicBleepInGameButtons(2);
	this->playMusicBleepInGameButtons(3);
	this->playMusicBleepInGameButtons(4);

	/* 鼠标移动到按钮式更换图片 */
	_menuItem[0]->getBoundingBox().containsPoint(Vec2(_cur.x - 606, _cur.y)) ? 
		_menuItem[0]->setNormalImage(Sprite::createWithSpriteFrameName("SelectorScreen_Options2.png")) :
		_menuItem[0]->setNormalImage(Sprite::createWithSpriteFrameName("SelectorScreen_Options1.png"));
	_menuItem[1]->getBoundingBox().containsPoint(Vec2(_cur.x - 606, _cur.y)) ?
		_menuItem[1]->setNormalImage(Sprite::createWithSpriteFrameName("SelectorScreen_Help2.png")) :
		_menuItem[1]->setNormalImage(Sprite::createWithSpriteFrameName("SelectorScreen_Help1.png"));
	_menuItem[2]->getBoundingBox().containsPoint(Vec2(_cur.x - 606, _cur.y)) ?
		_menuItem[2]->setNormalImage(Sprite::createWithSpriteFrameName("SelectorScreen_Quit2.png")) :
		_menuItem[2]->setNormalImage(Sprite::createWithSpriteFrameName("SelectorScreen_Quit1.png"));
	_menuItem[3]->getBoundingBox().containsPoint(_cur) ?
		_menuItem[3]->setNormalImage(Sprite::createWithSpriteFrameName("SelectorScreen_WoodSign2_press.png")) :
		_menuItem[3]->setNormalImage(Sprite::createWithSpriteFrameName("SelectorScreen_WoodSign2.png"));
	
	if (_global->userInformation->getIsUpdate())/* 如果名字更新 */
	{
		_userText->setString(_global->userInformation->getUserName());
		this->scheduleOnce(schedule_selector(MainMenu::updateUserNameOnce), 0);
		_global->userInformation->setIsUpdate(false);
	}
}

void MainMenu::updateUserNameOnce(float Time)
{
	/* 如果有孩子 */
	if (_userNameActionParent)
	{
		for (int i = 0; i < 10; i++)
		{
			_userNameActionParent->removeChildByTag(1000);
		}
	}

	srand(time(nullptr));
	/* 获取名字长度 */
	auto const len = _userText->getStringLength();
	for (int i = 0; i < len; i++)
	{
		auto userNameAction = _userText->getLetter(i);
		if (userNameAction && len > 3)
		{
			userNameAction->setTag(1000);
			_userNameActionParent = userNameAction->getParent();
			userNameAction->setColor(Color3B(rand() % 256, rand() % 256, rand() % 256));
			userNameAction->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1 * ((i + 1) % (len - 3))), Spawn::create(RotateBy::create(1.0f, 360), JumpBy::create(1.0f, Vec2(0, 0), 30, 1), NULL), DelayTime::create(1.5f), NULL)));
		}
	}
}

void MainMenu::playMusicBleepInGameButtons(int ID)
{
	/* 如果鼠标在按钮上 */
	if (this->checkCurInButtons() == ID)
	{
		_mainButton[ID]->setColor(Color3B::WHITE);
		/* 如果没有播放音乐 */
		if (!_playMusic[ID - 1])
		{
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("bleep")->second), _global->userInformation->getSoundEffectVolume());
			_playMusic[ID - 1] = true;
		}
	}
	else
	{
		if (_playMusic[ID - 1])  _playMusic[ID - 1] = false;
		_mainButton[ID]->setColor(Color3B(150, 150, 150));
	}
}

void MainMenu::playMusicBleepInMainBuutons(int ID,const Vec2 &vec2)
{
	const string image[][4]
	{
		{"SelectorScreen_Options2","SelectorScreen_Help2","SelectorScreen_Quit2","SelectorScreen_WoodSign2_press"},
		{"SelectorScreen_Options1","SelectorScreen_Help1","SelectorScreen_Quit1","SelectorScreen_WoodSign2"}
	};

	if (_menuItem[ID]->getBoundingBox().containsPoint(vec2))
	{
		/* 判断按钮编号 */
		_menuItem[ID]->setNormalImage(Sprite::createWithSpriteFrameName(image[0][ID] + ".png"));

		/* 如果没有播放音乐 */
		if (!_playMusic[ID + 4])
		{
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("bleep")->second), _global->userInformation->getSoundEffectVolume());
			_playMusic[ID + 4] = true;
		}
	}
	else
	{
		if (_playMusic[ID + 4]) _playMusic[ID + 4] = false;
		_menuItem[ID]->setNormalImage(Sprite::createWithSpriteFrameName(image[1][ID] + ".png"));
	}
}

short MainMenu::checkCurInButtons()
{
	/* 检测鼠标的位置是否在按钮上 */
	if ((_cur.x >= 1210 && _cur.x <= 1800 && _cur.y >= 800 && _cur.y <= 900) || (_cur.x >= 1210 && _cur.x <= 1600 && _cur.y >= 900 && _cur.y <= 930) ||
		(_cur.x >= 1580 && _cur.x <= 1790 && _cur.y >= 730 && _cur.y <= 800) || (_cur.x >= 1390 && _cur.x <= 1580 && _cur.y >= 745 && _cur.y <= 800))
	{
		return 1;
	}
	else if ((_cur.x >= 1210 && _cur.x <= 1770 && _cur.y >= 630 && _cur.y <= 700) || (_cur.x >= 1210 && _cur.x <= 1500 && _cur.y >= 700 && _cur.y <= 730) ||
		(_cur.x >= 1210 && _cur.x <= 1400 && _cur.y >= 730 && _cur.y <= 740) || (_cur.x >= 1210 && _cur.x <= 1350 && _cur.y >= 740 && _cur.y <= 760) ||
		(_cur.x >= 1500 && _cur.x <= 1600 && _cur.y >= 700 && _cur.y <= 720) || (_cur.x >= 1350 && _cur.x <= 1760 && _cur.y >= 610 && _cur.y <= 630) ||
		(_cur.x >= 1450 && _cur.x <= 1750 && _cur.y >= 590 && _cur.y <= 610) || (_cur.x >= 1550 && _cur.x <= 1750 && _cur.y >= 570 && _cur.y <= 590) ||
		(_cur.x >= 1660 && _cur.x <= 1750 && _cur.y >= 550 && _cur.y <= 570))
	{
		return 2;
	}
	else if ((_cur.x >= 1220 && _cur.x <= 1730 && _cur.y >= 500 && _cur.y <= 535) || (_cur.x >= 1220 && _cur.x <= 1600 && _cur.y >= 535 && _cur.y <= 560) ||
		(_cur.x >= 1220 && _cur.x <= 1450 && _cur.y >= 560 && _cur.y <= 590) || (_cur.x >= 1220 && _cur.x <= 1350 && _cur.y >= 590 && _cur.y <= 610) ||
		(_cur.x >= 1350 && _cur.x <= 1730 && _cur.y >= 480 && _cur.y <= 500) || (_cur.x >= 1430 && _cur.x <= 1720 && _cur.y >= 460 && _cur.y <= 480) ||
		(_cur.x >= 1520 && _cur.x <= 1720 && _cur.y >= 440 && _cur.y <= 460) || (_cur.x >= 1590 && _cur.x <= 1710 && _cur.y >= 420 && _cur.y <= 440))
	{
		return 3;
	}
	else if ((_cur.x >= 1230 && _cur.x <= 1300 && _cur.y >= 380 && _cur.y <= 485) || (_cur.x >= 1300 && _cur.x <= 1380 && _cur.y >= 360 && _cur.y <= 470) ||
		(_cur.x >= 1380 && _cur.x <= 1400 && _cur.y >= 350 && _cur.y <= 460) || (_cur.x >= 1400 && _cur.x <= 1500 && _cur.y >= 330 && _cur.y <= 440) ||
		(_cur.x >= 1500 && _cur.x <= 1550 && _cur.y >= 310 && _cur.y <= 430) || (_cur.x >= 1550 && _cur.x <= 1650 && _cur.y >= 290 && _cur.y <= 410) ||
		(_cur.x >= 1650 && _cur.x <= 1690 && _cur.y >= 280 && _cur.y <= 400))
	{
		return 4;
	}
	else
	{
		_mainButton[1]->setPosition(Vec2(900, 830));
		_mainButton[2]->setPosition(Vec2(880, 650));
		_mainButton[3]->setPosition(Vec2(870, 510));
		_mainButton[4]->setPosition(Vec2(850, 385));
		return 0;
	}
		
}

void MainMenu::createParticle()
{
	/* 创建烟雾 */
	this->createSmoke(0.3f, Vec2(87, 450));
	this->createSmoke(0.8f, Vec2(350, 690));
	this->createSmoke(0.1f, Vec2(15, 340));
	
	/* 创建雨 */
	auto Rain = ParticleRain::create();
	Rain->setEmissionRate(100);
	Rain->setStartSizeVar(5);
	Rain->setEndSizeVar(5);
	Rain->setLifeVar(20);
	Rain->setStartColorVar(Color4F(1, 1, 1, 1));
	Rain->setEndColor(Color4F::YELLOW);
    this->addChild(Rain, 1);

	
	/*auto my = ParticleSystemQuad::create("particle_texture.plist");
	my->setPosition(Vec2(300, 400));
	this->addChild(my, 10);*/
}

void MainMenu::createSmoke(const float &Scale,const Vec2 &vec2)
{
	/* 创建烟雾粒子特效 */
	auto Smoke = ParticleSmoke::create();
	Smoke->setScale(Scale);
	Smoke->setPosition(vec2);
	_sprite[1]->addChild(Smoke, -1);
}

void MainMenu::createFlowers()
{
	this->createFlowers(1.5f, Vec2(1230, 220), "SelectorScreen_Flower3", 3);
	this->createFlowers(2.0f, Vec2(1064, 260), "SelectorScreen_Flower2", 1);
	this->createFlowers(1.5f, Vec2(1130, 290), "SelectorScreen_Flower1", 2);
}

void MainMenu::createFlowers(const float &Scale, const Vec2 &vec2, const std::string &fileName, const int &ID)
{
	/* 创建花朵 */
	auto Flower = Sprite::createWithSpriteFrameName(fileName + ".png");
	Flower->setScale(Scale);
	Flower->setPosition(vec2);
	_sprite[3]->addChild(Flower, 1);

	/* 创建监听 */
	auto listener = EventListenerTouchOneByOne::create();
	/* 触摸函数 */
	listener->onTouchBegan = [=](Touch *t, Event *e)
	{
		auto points = t->getLocation();
		points.x -= 606;// x轴减去父图的相对坐标
		if (Flower->getBoundingBox().containsPoint(points))
		{
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("shoop")->second), _global->userInformation->getSoundEffectVolume());
			switch (ID)
			{
			case 1:
			{
				auto point = PointArray::create(18);
				Vec2 vec[] = { {1064, 260},{1044, 230},{1024, 240},{1044, 210},{1064, 220},{1044, 190},{1024, 200},{1044, 170},
							   {1064, 180},{1044, 150},{1024, 160},{1044, 130},{1064, 140},{1044, 110},{1024, 120},{1044, 90},
							   {1064, 100},{1044, 70} };
				for (int i = 0; i < 18; i++)
				{
					point->addControlPoint(vec[i]);
				}
				Flower->runAction(Spawn::create(CardinalSplineTo::create(4.0f, point, 0), RotateBy::create(4.0f, 360), NULL));
			}
			break;
			case 2:
				Flower->runAction(Spawn::create(RotateBy::create(1.0f, 180), MoveBy::create(1.0f, Vec2(300, 10)), NULL));
				break;
			case 3:
				Flower->runAction(Spawn::create(RotateBy::create(5.0f, 360), JumpBy::create(5.0f, Vec2(-1200, 400), 300, 1), ScaleTo::create(5.0f, 0), NULL));
				break;
			}
			
			listener->setEnabled(false);
		}
		return false;
	};

	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, Flower);
}


void MainMenu::createAnimation()
{
	/* 创建小精灵动画 */
	auto iter = _global->userInformation->getAnimationData().find("powerup");
	if (iter != _global->userInformation->getAnimationData().end())/* 如果可以找到 */
	{
		auto Littlesprite = SkeletonAnimation::createWithData(iter->second);
		Littlesprite->setPosition(Vec2(300, 270));
		Littlesprite->setAnimation(0, "animation", true);
		this->addChild(Littlesprite);
	}
	
	/* 创建叶子动画 */
	iter = _global->userInformation->getAnimationData().find("SelectorScreen_Leaves");
	if (iter != _global->userInformation->getAnimationData().end())
	{
		auto Leaves = SkeletonAnimation::createWithData(iter->second);
		Leaves->setAnimation(0, "Leaves_action", 1);
		Leaves->setPosition(Vec2(-270, -110));
		Leaves->setAnchorPoint(Vec2(0, 0));
		Leaves->setScale(2.7f);
		Leaves->setTimeScale(0.5f);
		_sprite[3]->addChild(Leaves);
	}

	/* 创建僵尸动画 */
	iter = _global->userInformation->getAnimationData().find("Zombies");
	if (iter != _global->userInformation->getAnimationData().end())
	{
		auto Zombies = SkeletonAnimation::createWithData(iter->second);
		auto Zombies1 = SkeletonAnimation::createWithData(iter->second);
		Zombies->setAnimation(0, "Zombies_Stand", 1);
		Zombies1->setAnimation(0, "Zombies_Stand1", 1);
		Zombies->setPosition(Vec2(880, 400));
		Zombies1->setPosition(Vec2(720, 450));
		Zombies->setAnchorPoint(Vec2(0, 0));
		Zombies->setScale(0.6f);
		Zombies1->setScale(0.5f);
		_sprite[1]->addChild(Zombies);
		_sprite[1]->addChild(Zombies1);
	}

	/* 创建植物动画 */
	iter = _global->userInformation->getAnimationData().find("PeaShooter");
	if (iter != _global->userInformation->getAnimationData().end())
	{
		auto PeaShooter = SkeletonAnimation::createWithData(iter->second);
		PeaShooter->setAnimation(0, "PeaShooter_Normal", 1);
		PeaShooter->setPosition(Vec2(450, 330));
		PeaShooter->setAnchorPoint(Vec2(0, 0));
		PeaShooter->setScale(0.6f);
		_sprite[1]->addChild(PeaShooter);
	}
}

void MainMenu::createMouseListener()
{
	/* 创建鼠标监听 */
	_mouse = EventListenerMouse::create();

	/* 移动监听 */
	_mouse->onMouseMove = [=](Event *event) 
	{
		_cur = ((EventMouse*)event)->getLocationInView(); 
	};

	/* 鼠标按下监听 */
	_mouse->onMouseDown = [&](Event *event) 
	{
		switch (this->checkCurInButtons())
		{
		case 1: 
			_mainButton[1]->setPosition(Vec2(902, 828)); /* 冒险模式 */
			break;
		case 2: 
			_mainButton[2]->setPosition(Vec2(882, 648)); /* 解迷模式 */
			break;
		case 3:
			_mainButton[3]->setPosition(Vec2(872, 508)); /* 玩玩小游戏 */
			break;
		case 4:
			_mainButton[4]->setPosition(Vec2(852, 383)); /* 生存模式 */
			break;
		}
		AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("gravebutton")->second), _global->userInformation->getSoundEffectVolume());
	};

	/* 鼠标松开监听 */
	_mouse->onMouseUp = [&](Event* event)
	{
		switch (this->checkCurInButtons())
		{
		case 1:
			_mainButton[1]->setPosition(Vec2(900, 830));
			beginAdventureGame(); /* 冒险模式 */
			break;
		case 2:
			_mainButton[2]->setPosition(Vec2(880, 650));
			beginSurvivalGame();  /* 解迷模式 */
			break;
		case 3:
			_mainButton[3]->setPosition(Vec2(870, 510));
			beginChallengesGame(); /* 玩玩小游戏 */
			break;
		case 4:
			_mainButton[4]->setPosition(Vec2(850, 385));
			beginVasebreakerGame(); /* 生存模式 */
			break;
		}
	};

	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_mouse, this);
}

void MainMenu::createMainButton()
{
	/* 创建按钮 */
	_menuItem[0] = MenuItemImage::create("","", CC_CALLBACK_1(MainMenu::menuOptionCallBack, this));
	_menuItem[1] = MenuItemImage::create("","", CC_CALLBACK_1(MainMenu::menuHelpCallBack, this));
	_menuItem[2] = MenuItemImage::create("","", CC_CALLBACK_1(MainMenu::menuQuitCallBack, this));
	_menuItem[3] = MenuItemImage::create("","", CC_CALLBACK_1(MainMenu::menuDataCallBack, this));

	_menuItem[0]->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("SelectorScreen_Options1"));
	_menuItem[1]->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("SelectorScreen_Help1"));
	_menuItem[2]->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("SelectorScreen_Quit1"));
	_menuItem[3]->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("SelectorScreen_WoodSign2"));

	_menuItem[0]->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("SelectorScreen_Options2"));
	_menuItem[1]->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("SelectorScreen_Help2"));
	_menuItem[2]->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("SelectorScreen_Quit2"));
	_menuItem[3]->setSelectedSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("SelectorScreen_WoodSign2_press"));

	_menuItem[0]->setCallback(CC_CALLBACK_1(MainMenu::menuOptionCallBack, this));
	_menuItem[1]->setCallback(CC_CALLBACK_1(MainMenu::menuHelpCallBack, this));
	_menuItem[2]->setCallback(CC_CALLBACK_1(MainMenu::menuQuitCallBack, this));
	_menuItem[3]->setCallback(CC_CALLBACK_1(MainMenu::menuDataCallBack, this));

	_menuItem[0]->setPosition(Vec2(960, 175));
	_menuItem[1]->setPosition(Vec2(1080, 130));
	_menuItem[2]->setPosition(Vec2(1210, 140));
	_menuItem[3]->setPosition(Vec2(145, 1080));

	_menuItem[0]->setScale(1.5f);
	_menuItem[1]->setScale(1.5f);
	_menuItem[2]->setScale(1.5f);
	_menuItem[3]->setScale(2.0f);

	_menuItem[3]->setAnchorPoint(Vec2(0, 0));
	_menuItem[3]->runAction(Sequence::create(DelayTime::create(0.8f), EaseElasticOut::create(MoveBy::create(1.1f, Vec2(0, -400))), NULL));

	_menu[0] = Menu::create(_menuItem[0], _menuItem[1], _menuItem[2], NULL);
	_menu[1] = Menu::create(_menuItem[3], NULL);
	
	_menu[0]->setPosition(Vec2(0, 0));
	_menu[1]->setPosition(Vec2(0, 0));

	_sprite[3]->addChild(_menu[0]);
	this->addChild(_menu[1]);
}

void MainMenu::createMainSprite()
{
	const string String[] =
	{
		{"SelectorScreen_BG"},
		{"SelectorScreen_BG1"},
		{"SelectorScreen_BG2"},
		{"SelectorScreen_BG3"},
		{"SelectorScreen_BG4"},
		{"SelectorScreen_BG5"},
	    {"SelectorScreen_BG_Center"},
	    {"SelectorScreen_BG_Left"},
		{"SelectorScreen_BG_Right"},
	    {"SelectorScreen_WoodSign1"},
	    {"SelectorScreen_WoodSign3"},
	};
	srand(time(nullptr));
	_sprite[0] = Sprite::createWithSpriteFrameName(String[rand() % 6] + ".png");
	for (int i = 6; i < 11; i++)
	{
		_sprite[i - 5] = Sprite::createWithSpriteFrameName(String[i] +".png");
	}

	Size size[] = { {1920, 1080},{1440, 700},{785, 1140},{1314, 1008},{586, 300},{184, 80} };
	for (int i = 0; i < 6; i++)
	{
		_sprite[i]->setContentSize(size[i]);
		_sprite[i]->setAnchorPoint(Vec2(0, 0));
	}
    this->addChild(_sprite[0], -1); //天空
	
	this->createClouds();

	this->createParticle();

	Vec2 vec2[] = { {300, -100},{0, -60},{606, -1000},{150, 1200} ,{150, 1080} };
	int number[] = { 1,2,3,4,5 };
	for (int i = 1; i < 6; i++)
	{
		_sprite[i]->setPosition(vec2[i - 1]);
		i == 5 ? this->addChild(_sprite[number[i - 1]], 1) : this->addChild(_sprite[number[i - 1]]);
	}

	_sprite[1]->runAction(MoveBy::create(0.5f, Vec2(0, 60)));
	_sprite[2]->runAction(MoveBy::create(0.5f, Vec2(0, 60)));
	_sprite[3]->runAction(MoveBy::create(0.5f, Vec2(0, 1000)));
	_sprite[4]->runAction(Sequence::create(DelayTime::create(0.8f), EaseElasticOut::create(MoveBy::create(1.0f, Vec2(0, -400))), nullptr));
	_sprite[5]->runAction(Sequence::create(DelayTime::create(0.8f), EaseElasticOut::create(MoveBy::create(1.15f, Vec2(0, -440))), nullptr));

	/* 创建用户名称 */
	_userText = Text::create();
	_userText->setFontName("resources/fonts/GameFont.ttf");
	_userText->setFontSize(30);
	_userText->setTextColor(Color4B::YELLOW);
	_userText->setPosition(Vec2(300, 90));
	_sprite[4]->addChild(_userText);

	/* 如果用户名为空的话 */
	_global->userInformation->getUserName().empty()? _userText->setString(_global->userInformation->getGameText().find("愤怒的小僵尸")->second) : _userText->setString(_global->userInformation->getUserName());
		
	this->scheduleOnce(schedule_selector(MainMenu::updateUserNameOnce), 0);

	_mainButton[1] = Sprite::createWithSpriteFrameName("SelectorScreen_Adventure_button.png");
	_mainButton[2] = Sprite::createWithSpriteFrameName("SelectorScreen_Survival_button.png");
	_mainButton[3] = Sprite::createWithSpriteFrameName("SelectorScreen_Challenges_button.png");
	_mainButton[4] = Sprite::createWithSpriteFrameName("SelectorScreen_Vasebreaker_button.png");
	_mainButton[5] = Sprite::createWithSpriteFrameName("SelectorScreen_Shadow_Adventure.png");
	_mainButton[6] = Sprite::createWithSpriteFrameName("SelectorScreen_Shadow_Survival.png");
	_mainButton[7] = Sprite::createWithSpriteFrameName("SelectorScreen_Shadow_Challenge.png");
	_mainButton[8] = Sprite::createWithSpriteFrameName("SelectorScreen_Shadow_ZenGarden.png");

	Vec2 Point[] = { {0,0},{900,830} ,{880,650},{870,510},{850,385},{910,826} ,{890,644},{880,506},{864,377} };
	for (int i = 8; i > 0; i--)
	{
		_mainButton[i]->setPosition(Point[i]);
		_mainButton[i]->setScale(1.8f);
		_sprite[3]->addChild(_mainButton[i]);
	}
}

void MainMenu::createClouds()
{
	srand(unsigned(time(NULL)));
	const string String[] =
	{
		{"SelectorScreen_Cloud1"},{"SelectorScreen_Cloud2"},
	    {"SelectorScreen_Cloud3"},{"SelectorScreen_Cloud4"},
	    {"SelectorScreen_Cloud5"},{"SelectorScreen_Cloud6"},
	    {"SelectorScreen_Cloud7"},{"SelectorScreen_Cloud8"},
	    {"SelectorScreen_Cloud9"},{"SelectorScreen_Cloud10"},
	    {"SelectorScreen_Cloud11"},{"SelectorScreen_Cloud12"}
	};
	Vec2 vec2[] = { {2500,1000},{2800,900},{2200,800},{2100,950},{3000,600},{2700,700},{2600,800},{3000,550},{2000,670},{2480,780},{2150,870},{2341,813} };
	Vec2 vec2_[] = { {0,1000},{0,900},{0,800},{0,950},{0,600},{0,700},{0,800},{0,550},{0,670},{0,780},{0,870},{0,813} };
	int Rotation[] = { 20,25,35,35,30,0,0,0,0,0,-20,-20 };
	for (int i = 6; i < 18; i++)
	{
		_sprite[i] = Sprite::createWithSpriteFrameName(String[i - 6] + ".png");
		_sprite[i]->setPosition(300 + rand() % 1800, vec2[i - 6].y);
		_sprite[i]->setRotation(Rotation[i - 6]);
		_sprite[i]->setScale(1 + (rand() % 3) / 1.2f);
		_sprite[i]->runAction(RepeatForever::create(Sequence::create(MoveTo::create(60 + rand() % 60, vec2_[i - 6]), CallFuncN::create(CC_CALLBACK_1(MainMenu::setCloudPosition, this, i, vec2[i - 6])), NULL)));
		this->addChild(_sprite[i]);
	}
}

void MainMenu::setCloudPosition(Node *node,int ID,const Vec2 &vec2)
{
	/* 初始化云 */
	_sprite[ID]->setPosition(vec2);
	_sprite[ID]->setScale(1 + (rand() % 3) / 1.2f);
}

void MainMenu::menuDataCallBack(Ref* pSender)
{
	AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap")->second), _global->userInformation->getSoundEffectVolume());

	setMouseListenerEnable(false);
	_inputLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	auto input = InputDataMenu::create();
	if (input)
	{
		input->addLayer(_inputLayer);
		input->setMouseListener(_mouse);
		this->addChild(_inputLayer, 1, "_inputLayer");
	}
}

void MainMenu::menuOptionCallBack(Ref* pSender)
{
	AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap")->second), _global->userInformation->getSoundEffectVolume());
	
	setMouseListenerEnable(false);
	_optionLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	auto option = OptionsMenu::create();
	if (option)
	{
		option->addLayer(_optionLayer);
		option->setMouseListener(_mouse);
		this->addChild(_optionLayer, 1, "_optionLayer");
	}
}

void MainMenu::menuQuitCallBack(Ref* pSender)
{
	AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap2")->second), _global->userInformation->getSoundEffectVolume());

	setMouseListenerEnable(false);

	_quitLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	auto quit = QuitMenu::create();
	if (quit)
	{
		quit->addLayer(_quitLayer);
		quit->setMouseListener(_mouse);
		this->addChild(_quitLayer, 1, "_quitLayer");
	}
}

void MainMenu::menuHelpCallBack(Ref* pSender)
{
	AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap2")->second), _global->userInformation->getSoundEffectVolume());
	Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, HelpScene::createHelpScene()));
}

void MainMenu::beginAdventureGame()
{
	_global->userInformation->setMainToWorld(true);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f,SelectWorldScene::createScene()));
}

void MainMenu::beginSurvivalGame()
{
	Application::getInstance()->openURL("https://share.weiyun.com/5TewoDc");
}

void MainMenu::beginChallengesGame()
{
	Application::getInstance()->openURL("https://share.weiyun.com/5TewoDc");
}

void MainMenu::beginVasebreakerGame()
{
	Application::getInstance()->openURL("https://pan.baidu.com/s/1NlpRcQTv1FrN51UgAzGiHw&shfl=sharepset#/");
}