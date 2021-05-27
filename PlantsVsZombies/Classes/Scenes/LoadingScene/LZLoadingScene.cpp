/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.15
 *Emal: 2117610943@qq.com
 */

#include "AudioEngine.h"
#include "tinyxml2/tinyxml2.h"

#include "LZLoadingScene.h"
#include "LZSelectLanguageLayer.h"
#include "Based/LZLevelData.h"
#include "Based/LZUserInformation.h"
#include "Based/LZPlayMusic.h"

#define MYDEBUG 1

bool LoadingScene::loadingFinished = false;

LoadingScene::LoadingScene() :
	_loadFileNumbers(0),
	_textNumbers(0),
	_allFileNumbers(0),
	_musicNumbers(0),
	_animationNumbers(0),
	_imageNumbers(0),
	_delayTime(0.15f),
	_loadingPrecent(0),
	_label(nullptr),
	_loadingBar(nullptr),
	_listener(nullptr),
	_nowtime(nullptr),
	_global(Global::getInstance()),
	_director(Director::getInstance()),
	_files(FileUtils::getInstance()),
	_userData(UserData::getInstance())
{
	_downloader.reset(new network::Downloader());
}

LoadingScene::~LoadingScene()
{
	SpriteFrameCache::getInstance()->
		removeSpriteFramesFromFile("resources/Images/LoadingScene/LoadingScene.plist");
	if (_nowtime)delete _nowtime;
}

bool LoadingScene::init()
{
	if (!Scene::init())return false;
	
#if MYRELEASE
#   if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	checkEdition();          /* 检查版本 */
#   endif
#endif
	setRunFirstTime();       /* 获取第一次运行时间 */
	setSystem();             /* 设置系统参数 */
	loadUserData();          /* 加载用户信息 */
	showLoadingBackGround(); /* 展示加载界面 */
	calculateFileNumbers();  /* 计算文件总数 */

	return true;
}

void LoadingScene::setSystem()
{
	/* 设置光标 */
	_director->getOpenGLView()->setCursor("resources/Images/System/cursor.png", Point::ANCHOR_TOP_LEFT);

	/* 设置图标 */
	_director->getOpenGLView()->setIcon("resources/Images/System/PlantsVsZombies.png");

}

void LoadingScene::loadUserData()
{
	auto userdefault = UserDefault::getInstance();

	/* 读取用户存档名称 */
	for (int i = 0; i < 8; i++)
	{
		if (!userdefault->getStringForKey(_global->userInformation->getUserCaveFileNameKey(i).c_str()).size())
		{
			_global->userInformation->setUserCaveFileName(i, "NoFile");
		}
		else
		{
			_global->userInformation->setUserCaveFileName(i, userdefault->getStringForKey(_global->userInformation->getUserCaveFileNameKey(i).c_str()));
		}
	}

	_userData->createNewUserDataDocument();
	loadUserFileData();

	/* 用户名称 */
	_global->userInformation->setUserName(_global->userInformation->getUserCaveFileName(_global->userInformation->getUserCaveFileNumber()));
	_global->userInformation->setIsUpdate(true);

	/* 显示信息 */
	switch (userdefault->getBoolForKey("SHOWINFORMATION"))
	{
	case true:
		_director->setDisplayStats(true);
		_global->userInformation->setIsShowInformation(cocos2d::ui::CheckBox::EventType::SELECTED);
		break;
	case false:
		_director->setDisplayStats(false);
		_global->userInformation->setIsShowInformation(cocos2d::ui::CheckBox::EventType::UNSELECTED);
		break;
	}

	/* 是否高帧率 */
	switch (userdefault->getBoolForKey("SHOWHIGHFPS"))
	{
	case true:
		_director->setAnimationInterval(1.0f / UserInformation::getScreenDisplayFrequency());
		_global->userInformation->setIsSelectHighFPS(cocos2d::ui::CheckBox::EventType::SELECTED);
		break;
	case false:
		_director->setAnimationInterval(1.0f / 30);
		_global->userInformation->setIsSelectHighFPS(cocos2d::ui::CheckBox::EventType::UNSELECTED);
		break;
	}

	/* 是否全屏 */
	switch (userdefault->getBoolForKey("SHOWFULLSCREEN"))
	{
	case true:
		((GLViewImpl*)_director->getOpenGLView())->setFullscreen();
		_global->userInformation->setIsSelectFullScreen(cocos2d::ui::CheckBox::EventType::SELECTED);
		break;
	case false:
		((GLViewImpl*)_director->getOpenGLView())->setWindowed(1280, 720);
		_global->userInformation->setIsSelectFullScreen(cocos2d::ui::CheckBox::EventType::UNSELECTED);
		break;
	}

	/* 是否拉伸显示 */
	switch (userdefault->getBoolForKey("STRETCHINGSHOW"))
	{
	case true:
		_global->userInformation->setIsSelectStretchingShow(cocos2d::ui::CheckBox::EventType::SELECTED);
		_director->getOpenGLView()->setDesignResolutionSize(_director->getWinSize().width, _director->getWinSize().height, ResolutionPolicy::EXACT_FIT);
		break;
	case false:
		_global->userInformation->setIsSelectStretchingShow(cocos2d::ui::CheckBox::EventType::UNSELECTED);
		_director->getOpenGLView()->setDesignResolutionSize(_director->getWinSize().width, _director->getWinSize().height, ResolutionPolicy::SHOW_ALL);
		break;
	}

	/* 是否垂直同步 */
	switch (userdefault->getBoolForKey("VERTICALSYNCHRONIZATION"))
	{
	case true:
		wglSwapIntervalEXT(1);
		_global->userInformation->setIsVerticalSynchronization(CheckBox::EventType::SELECTED);
		break;
	case false:
		_global->userInformation->setIsVerticalSynchronization(CheckBox::EventType::UNSELECTED);
		wglSwapIntervalEXT(0);
		break;
	}

	/* 是否隐藏鼠标 */
	_global->userInformation->setIsSelectCursorNotHide(_userData->openBoolUserData("CURSORHIDE") ?
		cocos2d::ui::CheckBox::EventType::SELECTED : cocos2d::ui::CheckBox::EventType::UNSELECTED);

	/* 是否显示缓入动画 */
	_global->userInformation->setIsEaseAnimation(_userData->openBoolUserData("EASEANIMATION") ?
		cocos2d::ui::CheckBox::EventType::SELECTED : cocos2d::ui::CheckBox::EventType::UNSELECTED);

#ifndef _DEBUG
	if (!UserDefault::getInstance()->getBoolForKey("CHANGEFILE"))
	{
		changeFiles();
		UserDefault::getInstance()->setBoolForKey("CHANGEFILE", true);
    }
#endif
}

void LoadingScene::loadUserFileData()
{
	if (!UserDefault::getInstance()->getBoolForKey("FIRSTRUNGAME"))
	{
		UserDefault::getInstance()->setFloatForKey("SOUNDEFFECT", 0.5f);
		UserDefault::getInstance()->setFloatForKey("GLOBALMUSIC", 0.2f);
		UserDefault::getInstance()->setBoolForKey("SHOWFULLSCREEN", true);
		UserDefault::getInstance()->setBoolForKey("SHOWHIGHFPS", true);
		UserDefault::getInstance()->setBoolForKey("SHOWINFORMATION", true);
		UserDefault::getInstance()->setBoolForKey("VERTICALSYNCHRONIZATION", true);
		UserDefault::getInstance()->setBoolForKey("FIRSTRUNGAME", true);
	}

	Global::getInstance()->userInformation->setUserCaveFileNumber(UserDefault::getInstance()->getIntegerForKey("USERDATANUMBER"));      /* 存档编号 */
	Global::getInstance()->userInformation->setSoundEffectVolume(UserDefault::getInstance()->getFloatForKey("SOUNDEFFECT"));            /* 音效 */
	Global::getInstance()->userInformation->setBackGroundMusicVolume(UserDefault::getInstance()->getFloatForKey("GLOBALMUSIC"));        /* 音乐 */

	if (!UserData::getInstance()->openBoolUserData("FIRSTRUNGAME"))
	{
		UserData::getInstance()->caveUserData("EASEANIMATION", true);
		UserData::getInstance()->caveUserData("CURSORHIDE", true);
		UserData::getInstance()->caveUserData("JUMPLEVELNUMBERS", 5);
		UserData::getInstance()->caveUserData("FIRSTRUNGAME", true);
	}

	Global::getInstance()->userInformation->setKillZombiesNumbers(UserData::getInstance()->openIntUserData("KILLALLZOMBIES"));          /* 杀死僵尸数 */
	Global::getInstance()->userInformation->setUsePlantsNumbers(UserData::getInstance()->openIntUserData("USEPLANTSNUMBERS"));          /* 使用植物数量 */
	Global::getInstance()->userInformation->setIsShowEggs(UserData::getInstance()->openBoolUserData("ISBEGINSHOWEGGS"));                /* 显示彩蛋 */
	Global::getInstance()->userInformation->setCoinNumbers(UserData::getInstance()->openIntUserData("COINNUMBERS"));                    /* 金币数 */
	Global::getInstance()->userInformation->setBreakThroughNumbers(UserData::getInstance()->openIntUserData("BREAKTHROUGH"));           /* 闯关失败个数 */

	Global::getInstance()->userInformation->newUserSelectWorldData();
}

void LoadingScene::caveUserFileData()
{
	UserData::getInstance()->caveUserData("KILLALLZOMBIES", Global::getInstance()->userInformation->getKillZombiesNumbers());
	UserData::getInstance()->caveUserData("USEPLANTSNUMBERS", Global::getInstance()->userInformation->getUsePlantsNumbers());
	UserData::getInstance()->caveUserData("BREAKTHROUGH", Global::getInstance()->userInformation->getBreakThroughNumbers());
	UserData::getInstance()->caveUserData("ISBEGINSHOWEGGS", Global::getInstance()->userInformation->getIsShowEggs());
	UserData::getInstance()->caveUserData("COINNUMBERS", Global::getInstance()->userInformation->getCoinNumbers());
}

void LoadingScene::showLoadingBackGround()
{
	/* 获取窗口大小 */
	auto const size = Director::getInstance()->getWinSize();

	/* 创建精灵 */
	_sprite[0] = Sprite::createWithSpriteFrameName("Cocos2dx_Logo.png");
	_sprite[1] = Sprite::createWithSpriteFrameName("PopCap_Logo.png");
	_sprite[7] = Sprite::create("resources/Text/About.reanim.compiled");
	_sprite[2] = Sprite::createWithSpriteFrameName("titlescreen.png");

	/* 为精灵设置名字 */
	_sprite[0]->setName("0");
	_sprite[1]->setName("1");
	_sprite[7]->setName("7");

	/* 设置精灵大小 */
	_sprite[0]->setScale(1.5f);
	_sprite[1]->setScale(1.5f);
	_sprite[2]->setContentSize(size);
	_sprite[7]->setScale(1.5f);

	/* 设置精灵的位置 */
	_sprite[0]->setPosition(size / 2);
	_sprite[1]->setPosition(size / 2);
	_sprite[7]->setPosition(size / 2);
	_sprite[2]->setPosition(size / 2);

	/* 设置精灵透明度 */
	_sprite[0]->setOpacity(0);
	_sprite[1]->setOpacity(0);
	_sprite[7]->setOpacity(0);
	_sprite[2]->setOpacity(0);

	/* 把精灵加载到场景中 */
	this->addChild(_sprite[0]);
	this->addChild(_sprite[1]);
	this->addChild(_sprite[7]);
	this->addChild(_sprite[2]);

	/* 设置精灵不可见 */
	_sprite[1]->setVisible(false);
	_sprite[0]->setVisible(false);
	_sprite[2]->setVisible(false);

	/* 设置精灵动作 */
	if (!UserDefault::getInstance()->getBoolForKey("FIRSTRUN"))
	{
		UserDefault::getInstance()->setBoolForKey("FIRSTRUN", true);
		_sprite[7]->runAction(Sequence::create(FadeIn::create(2.f), DelayTime::create(8.f),
			FadeOut::create(5.f), CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 1)), nullptr));
	}
	else
	{
		_sprite[7]->runAction(Sequence::create(FadeIn::create(2.f),
			FadeOut::create(2.f), CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 1)), nullptr));
	}
}

void LoadingScene::runLoGoCallBack(Node* node, const int& ID)
{
	switch (ID)
	{
	case 1:
		this->removeChildByName("7"); /* 从场景中移除名字为5的孩子 */
		_sprite[0]->setVisible(true);  /* 设置精灵0可见 */
		_sprite[0]->runAction(Sequence::create(FadeIn::create(1.f),
			FadeOut::create(1.f), CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 5)), nullptr));
		break;
	case 2:
		this->removeChildByName("1"); /* 从场景中移除名字为7的孩子 */
		_sprite[2]->setVisible(true);  /* 设置精灵2可见 */
		_sprite[2]->runAction(Sequence::create(FadeIn::create(1.f), 
			CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 3)), nullptr));
		break;
	case 3:
		this->showTileAndLoadingBar(); /* 展示标题和进度条 */
		break;
	case 4:
		this->beginLoadingImageAndMusic(); /* 开始加载图片和音乐 */
		break;
	case 5:
		this->removeChildByName("0"); /* 从场景中移除名字为1的孩子 */
		_sprite[1]->setVisible(true);  /* 设置精灵1可见 */
		_sprite[1]->runAction(Sequence::create(FadeIn::create(1.f),
			FadeOut::create(1.f), CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 2)), nullptr));
		break;
	}
}

void LoadingScene::showTileAndLoadingBar()
{
	/* 获取窗口大小 */
	auto const size = _director->getWinSize();

	/* 创建精灵 */
	_sprite[3] = Sprite::createWithSpriteFrameName("PvZ_Logo.png");
	_sprite[4] = Sprite::createWithSpriteFrameName("LoadBar_dirt.png");
	_sprite[5] = Sprite::createWithSpriteFrameName("SodRollCap.png");
	_sprite[6] = Sprite::createWithSpriteFrameName("spark.png");

	/* 创建裁剪节点 */
	auto clippingNode = ClippingNode::create();

	/* 获取第三个精灵的大小 */
	Size const SpriteSize = _sprite[3]->getContentSize();

	/* 为精灵设置初始位置 */
	_sprite[4]->setPosition(Vec2(size.width / 2, -100));
	_sprite[5]->setPosition(Vec2(5, 95));
	_sprite[6]->setPosition(Vec2(-SpriteSize.width, 0));
	clippingNode->setPosition(Vec2(size.width / 2 + 10, 1100));

	/* 为精灵设置大小 */
	_sprite[3]->setScaleX(1.25f);
	_sprite[4]->setScale(2.0f);
	_sprite[6]->setScale(2.0f);


	/* 让精灵运动起来 */
	_sprite[4]->runAction(Sequence::create(
		EaseSineOut::create(MoveTo::create(0.5f, Vec2(size.width / 2, 150))),
		CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 4)), NULL));
	_sprite[6]->runAction(RepeatForever::create(
		Sequence::create(MoveTo::create(1.0f, Vec2(SpriteSize.width, 0)), DelayTime::create(2.0f),
		CallFunc::create([=]()
			{
				_sprite[6]->setPosition(Vec2(-SpriteSize.width, 0));
			}), nullptr)));
	clippingNode->runAction(EaseBounceOut::create(MoveTo::create(0.5f, Vec2(size.width / 2, 900))));

	/* 加到场景中 */
	this->addChild(_sprite[4]);
	this->addChild(clippingNode);
	_sprite[4]->addChild(_sprite[5], 1);

	/* 设置裁剪节点的基本属性 */
	clippingNode->setAlphaThreshold(0.01f);    //设置alpha闸值
	clippingNode->setContentSize(SpriteSize);  //设置尺寸大小
	clippingNode->setStencil(_sprite[3]);      //设置模板stencil
	clippingNode->addChild(_sprite[3]);        //先添加标题,会完全显示出来,因为跟模板一样大小
	clippingNode->addChild(_sprite[6]);        //会被裁减

	/* 创建菜单标签 */
	string text;
	switch (Application::getInstance()->getCurrentLanguage())
	{
	case LanguageType::CHINESE: text = "加载中......";   break;
	default:                    text = "Loading......";  break;
	}
	_label = MenuItemLabel::create(
		Label::createWithSystemFont(text, "language/fonts/GameFont.ttf", 18),
		CC_CALLBACK_1(LoadingScene::beginGameCallBack, this));
	_label->runAction(RepeatForever::create(Sequence::create(TintTo::create(0.5f, 255, 255, 255), TintTo::create(0.5f, 0, 0, 0), nullptr)));
	_label->setColor(Color3B::YELLOW);
	_label->setPosition(Vec2(0, -3));
	_label->setEnabled(false);

	/* 创建菜单 */
	auto menu = Menu::create(_label, NULL);
	menu->setPosition(160, 55);
	_sprite[4]->addChild(menu);
}

void LoadingScene::beginLoadingImageAndMusic()
{
	/* 创建进度条 */
	_loadingBar = LoadingBar::create();
	_loadingBar->loadTexture("LoadBar_grass.png", Widget::TextureResType::PLIST);
	_loadingBar->setDirection(LoadingBar::Direction::LEFT); /* 设置加载方向 */
	_loadingBar->setPosition(Vec2(150, 65));
	_sprite[4]->addChild(_loadingBar);

	/* 定时器 */
	scheduleUpdate();

	auto language = UserDefault::getInstance()->getStringForKey("LANGUAGE");
	if (language.empty())
	{
		switch (Application::getInstance()->getCurrentLanguage())
		{
		case LanguageType::CHINESE: language = "中文";     break;
		default:                    language = "english";  break;
		}
	}
	
	runAction(Sequence::create(
		CallFunc::create([=]() {loadingText(language.c_str()); /* 加载文本 */}),
		CallFunc::create([&]() {loadingAnimation();            /* 加载动画 */}),
		DelayTime::create(_animationNumbers * _delayTime),
		CallFunc::create([&]() {loadingImage();                /* 加载图片 */}),
		DelayTime::create(_imageNumbers * _delayTime),
		CallFunc::create([&]() {loadingMusic();                /* 加载音乐 */}),
		CallFunc::create([&]() {throwException();              /* 抛出异常 */}),
		nullptr));
}

void LoadingScene::update(float Time)
{
	if (_loadingPrecent <= 100)
	{
		_loadingBar->setPercent(_loadingPrecent);              /* 设置加载进度 */
		_sprite[5]->setScale(1 - _loadingPrecent / 170);       /* 设置精灵大小 */
		_sprite[5]->setRotation(9 * _loadingPrecent);          /* 设置精旋转度数 */
		_sprite[5]->setPosition(Vec2(5 + 290 / 100.0 * _loadingPrecent, 95 - _sprite[5]->getContentSize().height / 350 * _loadingPrecent));

		if (_loadingPrecent >= 20)  showLoadingBarFlower(0);
		if (_loadingPrecent >= 40)  showLoadingBarFlower(1);
		if (_loadingPrecent >= 60)  showLoadingBarFlower(2);
		if (_loadingPrecent >= 80)  showLoadingBarFlower(3);
		if (_loadingPrecent >= 100) showLoadingBarFlower(4);

	}
	if (_loadingPrecent >= 100 && !loadingFinished)            /* 如果加载完成 */
	{
		_label->setString(_global->userInformation->getGameText().find("点击开始")->second->text);  /* 重新设置标签文字内容 */
		auto action = TintBy::create(0.5f, 0, 255, 255);
		_label->runAction(RepeatForever::create(Sequence::create(action, action->reverse(), nullptr)));
		_sprite[5]->setVisible(false);  /* 设置精灵5为不可见 */
		_label->setEnabled(true);       /* 设置触摸可行 */

		loadingFinished = true;

		selectLanguage();
		COUNTPLAYERS
	}
}

void LoadingScene::showLoadingBarFlower(const int& ID)
{
	srand(unsigned(time(nullptr)));
	if (!_flowerVisible[ID]) /* 如果花的ID是不可见 */
	{
		/* 创建花 */
		auto flower = Sprite::createWithSpriteFrameName("sprout_petal.png");
		flower->setScale(0.1f);
		flower->setPosition(Vec2(20 + (55 + rand() % 10) * ID, 85 + rand() % 10));
		flower->runAction(ScaleTo::create(0.2f, 0.5f));
		_sprite[4]->addChild(flower);
		if (ID == 4) /* 如果ID==4 创建僵尸头*/
		{
			PlayMusic::playMusic("loadingbar_zombie");
			
			flower->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ZombieHead1.png"));
		}
		else  /* 否则创建花 */
		{
			PlayMusic::playMusic("loadingbar_flower");
			
			flower->setRotation(rand() % 180);
		}

		_flowerVisible[ID] = true; /* 花ID设置为可见 */
	}
}

void LoadingScene::calculateFileNumbers()
{
#if MYDEBUG
	/* 文件总数 = 文本数 + 图片数 + 音乐数 + 动画数 */
	_textNumbers = openResourcesPath(_global->userInformation->getTextPath(), "resources/Text/TextPath.reanim.compiled", true);
	_imageNumbers = openResourcesPath(_global->userInformation->getImagePath(), "resources/Text/ImagePath.reanim.compiled", true);
	_musicNumbers = openResourcesPath(_global->userInformation->getMusicPath(), "resources/Text/MusicPath.reanim.compiled", true);
	_animationNumbers = openResourcesPath(_global->userInformation->getAnimationPath(), "resources/Text/AnimationPath.reanim.compiled", true);
	_allFileNumbers = _textNumbers + _imageNumbers + _musicNumbers + _animationNumbers;
#else
	_textNumbers = openResourcesPath(_global->userInformation->getTextPath(), "resources/Text/TextPath.xml", false);
	_imageNumbers = openResourcesPath(_global->userInformation->getImagePath(), "resources/Text/ImagePath.xml", false);
	_musicNumbers = openResourcesPath(_global->userInformation->getMusicPath(), "resources/Text/MusicPath.xml", false);
	_animationNumbers = openResourcesPath(_global->userInformation->getAnimationPath(), "resources/Text/AnimationPath.xml", false);
	_allFileNumbers = _textNumbers + _imageNumbers + _musicNumbers + _animationNumbers;
#endif
}

void LoadingScene::setRunFirstTime()
{
	_nowtime = new MomentTime;
	_nowtime->requestNetTime([this]()
		{
			//CCLOG("%d 年 %d  月 %d 日  %d 星期 %d  时 %d 分 %d 秒 ", nowtime->tm_year + 1900, nowtime->tm_mon + 1, 
			//nowtime->tm_mday, nowtime->tm_wday, nowtime->tm_hour, nowtime->tm_min, nowtime->tm_sec);
			if (UserDefault::getInstance()->getStringForKey("FIRSTRUNTIME").size() == 0)
			{
				UserDefault::getInstance()->setStringForKey("FIRSTRUNTIME",
					to_string(_nowtime->getNetYear()) + "年 " +
					to_string(_nowtime->getNetMon()) + "月 " +
					to_string(_nowtime->getNetDay()) + "日 星期" +
					to_string(_nowtime->getNetWeek()) + " " +
					to_string(_nowtime->getNetHour()) + "时 " +
					to_string(_nowtime->getNetMin()) + "分 " +
					to_string(_nowtime->getNetSec()) + "秒");
			}

			UserDefault::getInstance()->setIntegerForKey("BEGINDAY", _nowtime->getNetDay());
			UserDefault::getInstance()->setIntegerForKey("BEGINHOUR", _nowtime->getNetHour());
			UserDefault::getInstance()->setIntegerForKey("BEGINMIN", _nowtime->getNetMin());
			UserDefault::getInstance()->setIntegerForKey("BEGINSEC", _nowtime->getNetSec());
		});
}

void LoadingScene::selectLanguage()
{
	auto lang = Button::create("ButtonNew.png", "ButtonNew2.png", "",cocos2d::ui::Widget::TextureResType::PLIST);
	lang->setTitleText(_global->userInformation->getGameText().find("语言")->second->text);
	lang->setTitleFontSize(_global->userInformation->getGameText().find("语言")->second->fontsize);
	lang->setTitleFontName(GAME_FONT_NAME_1);
	lang->setScale(0.5f);
	lang->setTitleColor(Color3B::WHITE);
	lang->setPosition(Vec2(_director->getWinSize().width / 2.0f, 50));
	lang->setLocalZOrder(10);
	addChild(lang);

	lang->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap");
				break;
			case ui::Widget::TouchEventType::ENDED:
				auto language = SelectLanguage::create();
				if (language)
					this->addChild(language, 11, "_language");
				break;
			}
		});
}

int LoadingScene::openResourcesPath(map<string, string>& Path, const std::string& xml, bool IsEncryption)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

	if (IsEncryption)
	{
		std::string textpath = _files->getStringFromFile(xml);
		char* passWords = (char*)malloc(sizeof(char) * textpath.size());

		OpenLevelData::getInstance()->decrypt(textpath, passWords);

		/* 打开资源路径 */
		doc->Parse(passWords);
	}
	else
	{
		/* 打开资源路径 */
		doc->Parse(_files->getStringFromFile(xml).c_str());
	}

	auto root = doc->RootElement();
	for (auto e = root->FirstChildElement(); e; e = e->NextSiblingElement())
	{
		for (auto at = e->FirstAttribute(); at; at = at->Next())
		{
			Path.insert(pair<string, string>(at->Name(), at->Value()));
		}
	}
	delete doc;
	return Path.size();
}

void LoadingScene::throwException()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	this->runAction(Sequence::create(DelayTime::create(60.f), CallFunc::create([=]()
		{
			try
			{
				if (_loadFileNumbers > 10 && _loadFileNumbers < _allFileNumbers)
				{
					wstring str = L"糟糕！发生了一些错误，部分资源文件加载失败！\n选择“重试”重新启动游戏，选择“取消”关闭游戏。";
					PlayMusic::playMusic("buzzer", false);
					throw str;
				}
			}
			catch (wstring str)
			{
				const auto yon = MessageBoxW(_director->getOpenGLView()->getWin32Window(), str.c_str(), L"资源加载异常", MB_RETRYCANCEL);
				/*if (yon == IDRETRY)
				{
					TCHAR szPath[MAX_PATH];
					GetModuleFileName(NULL, szPath, MAX_PATH);
					STARTUPINFO StartInfo;
					PROCESS_INFORMATION procStruct;
					memset(&StartInfo, 0, sizeof(STARTUPINFO));
					StartInfo.cb = sizeof(STARTUPINFO);
					if (!::CreateProcess((LPCTSTR)szPath, NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &StartInfo, &procStruct))return;
					Director::getInstance()->end();
				}
				else
				{
					Director::getInstance()->end();
				}*/
				Director::getInstance()->end();
			}
		}), nullptr));
#endif
}

void LoadingScene::checkEdition()
{
#if MYRELEASE
	const string sURLList = "https://gitee.com/GITLZ/PVZDownLoader/raw/master/edition.txt";
	_downloader->createDownloadDataTask(sURLList);
	_downloader->onDataTaskSuccess = [this](const cocos2d::network::DownloadTask& task,
		std::vector<unsigned char>& data)
	{
		string editionNetWork, editionNet;
		for (auto p : data)
		{
			editionNet += p;
			if (p != '.')
				editionNetWork += p;
		}
		
		if (std::stoul(UserInformation::getClientEdition()) < std::stoul(editionNetWork))
		{
			UserInformation::setUpdateRequired(true);
			UserInformation::setNewEditionName(editionNet);
		}
	};

	auto editionName = UserDefault::getInstance()->getStringForKey("EDITION");
	if (!editionName.empty())
	{
		if (std::stoul(UserInformation::getClientEdition()) < std::stoul(editionName))
		{
			UserInformation::setUpdateRequired(true);
			UserInformation::setNewEditionName(editionName);
		}
	}
#endif
}

void LoadingScene::changeFiles()
{
	auto str = FileUtils::getInstance()->fullPathForFilename("lzpvz.dll");
	const int lenbf = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), 0, 0);
	wchar_t* buf = new wchar_t[lenbf];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buf, sizeof(wchar_t) * lenbf);
	buf[str.size()] = 0;
	SetFileAttributes(buf, FILE_ATTRIBUTE_HIDDEN);
}

void LoadingScene::loadingText(const char* language)
{
	/* 循环加载文本 */
	for (auto& i : _global->userInformation->getTextPath())
	{
		if (i.second.find("language") != string::npos)
		{
			string str = FileUtils::getInstance()->getStringFromFile(i.second);

			rapidjson::Document d;
			d.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

			if (d.HasParseError()) continue;

			/* 读取语言 */
			if (d.HasMember(language))
			{
				for (unsigned int j = 0; j < d[language].Size(); ++j)
				{
					LanguageTextAttribute* lta = new LanguageTextAttribute;
					lta->text = d[language][j][1].GetString();
					lta->fontsize = d[language][j][2].GetFloat();
					lta->scale = d[language][j][3].GetFloat();
					lta->position.x = d[language][j][4].GetFloat();
					lta->position.y = d[language][j][5].GetFloat();

					_global->userInformation->getGameText().insert(pair<string, LanguageTextAttribute*>(d[language][j][0].GetString(), lta));
				}
			}
		}
		loadingTextCallBack();
	}
}

void LoadingScene::loadingImage()
{
	int number = 0;
	/* 循环加载图片 */
	for (auto& i : _global->userInformation->getImagePath())
	{
		runAction(Sequence::create(DelayTime::create(++number * _delayTime),
			CallFunc::create([=]()
				{
					_director->getTextureCache()->addImageAsync(i.second + "pvr.ccz", [=](Texture2D* texture)
						{
							SpriteFrameCache::getInstance()->addSpriteFramesWithFile(i.second + "plist", texture);
							_loadFileNumbers++;     /* 文件数加一 */
							_loadingPrecent = ((_loadFileNumbers * 1.0f) / _allFileNumbers) * 100;  /* 计算加载的百分比 */
						});
				}), nullptr));
	}
}

void LoadingScene::loadingMusic()
{
	int number = 0;
	/* 循环加载音乐 */
	for (auto& i : _global->userInformation->getMusicPath())
	{
		runAction(Sequence::create(DelayTime::create(++number * 0.015f),
			CallFunc::create([=]()
				{
					AudioEngine::preload(i.second, [=](bool isSucceed)
						{
							if (isSucceed)/* 如果加载成功 */
							{
								_loadFileNumbers++;     /* 文件数加一 */
								_loadingPrecent = ((_loadFileNumbers * 1.0f) / _allFileNumbers) * 100;  /* 计算加载的百分比 */
							}
						});
				}), nullptr));
		
	}
}

void LoadingScene::loadingAnimation()
{
	int number = 0;
	/* 循环加载动画 */
	for (auto& i : _global->userInformation->getAnimationPath())
	{
		runAction(Sequence::create(DelayTime::create(++number * _delayTime),
			CallFunc::create([=]() 
				{
					/* 加载 */
					auto json = spSkeletonJson_createWithLoader((spAttachmentLoader*)Cocos2dAttachmentLoader_create(
						spAtlas_createFromFile(("resources/Animations/reanim/" + i.second + ".reanim").c_str(), nullptr)));
					auto skeletonData = spSkeletonJson_readSkeletonDataFile(json, ("resources/Animations/compiled/" + i.second + ".compiled").c_str());
					spSkeletonJson_dispose(json);

					/* 把加载到的动画放入map中 */
					_global->userInformation->getAnimationData().insert(pair<string, spSkeletonData*>(i.second, skeletonData));

					/* 进行回调 */
					loadingAnimationCallBack();
				}), nullptr));
	}
}

void LoadingScene::loadingTextCallBack()
{
	_loadFileNumbers++;     /* 文件数加一 */
	_loadingPrecent = ((_loadFileNumbers * 1.0f) / _allFileNumbers) * 100;  /* 计算加载的百分比 */
}

void LoadingScene::loadingAnimationCallBack()
{
	_loadFileNumbers++;     /* 文件数加一 */
	_loadingPrecent = ((_loadFileNumbers * 1.0f) / _allFileNumbers) * 100;  /* 计算加载的百分比 */
}

void LoadingScene::beginGameCallBack(Ref* pSender)
{
	PlayMusic::playMusic("buttonclick");
	_director->replaceScene(MainMenu::create()); /* 切换场景 */
}