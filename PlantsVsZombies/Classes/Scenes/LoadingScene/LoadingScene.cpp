/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.15
 *Emal: 2117610943@qq.com
 */

#include "AudioEngine.h"
#include "Based/LevelData.h"
#include "LoadingScene.h"
#include "tinyxml2/tinyxml2.h"

#define MYDEBUG 1

LoadingScene::LoadingScene():
	_loadFileNumbers(0),
	_textNumbers(0),
	_allFileNumbers(0),
	_loadingPrecent(0),
	_label(nullptr),
	_loadingBar(nullptr),
	_listener(nullptr),
	_global(Global::getInstance()),
	_director(Director::getInstance()),
	_files(FileUtils::getInstance())
{
}

LoadingScene::~LoadingScene()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("resources/images/LoadingScene/LoadingScene.plist");
}

Scene* LoadingScene::createLaodingScene()
{
	return LoadingScene::create();
}

bool LoadingScene::init()
{
	if (!Scene::init())return false;
	
	setRunFirstTime();       /* 获取第一次运行时间 */
	calculateFileNumbers();  /* 计算文件总数 */
	setSystem();             /* 设置系统参数 */
	loadUserData();          /* 加载用户信息 */
	showLoadingBackGround(); /* 展示加载界面 */
	
	return true;
}

void LoadingScene::setSystem()
{
	/* 设置光标 */
	_director->getOpenGLView()->setCursor("resources/images/System/cursor.png", Point::ANCHOR_TOP_LEFT);

	/* 设置图标 */
	_director->getOpenGLView()->setIcon("resources/images/System/PlantsVsZombies.png");

}

void LoadingScene::loadUserData()
{
	auto userdefault = UserDefault::getInstance();

	_global->userInformation->setKillZombiesNumbers(userdefault->getIntegerForKey("KILLALLZOMBIES")); /* 杀死僵尸数 */
	_global->userInformation->setUsePlantsNumbers(userdefault->getIntegerForKey("USEPLANTSNUMBERS")); /* 使用植物数量 */
	_global->userInformation->setBreakThroughnumbers(userdefault->getIntegerForKey("BREAKTHROUGH"));  /* 闯关失败个数 */
	_global->userInformation->setUserCaveFileNumber(userdefault->getIntegerForKey("USERDATANUMBER")); /* 存档编号 */
	_global->userInformation->setIsShowEggs(userdefault->getBoolForKey("ISBEGINSHOWEGGS"));           /* 显示彩蛋 */
	_global->userInformation->setCoinNumbers(userdefault->getIntegerForKey("COINNUMBERS"));           /* 金币数 */
	_global->userInformation->setSoundEffectVolume(userdefault->getFloatForKey("SOUNDEFFECT"));       /* 音效 */
	_global->userInformation->setBackGroundMusicVolume(userdefault->getFloatForKey("GLOBALMUSIC"));   /* 音乐 */

	/* 读取用户存档名称 */
	for (int i = 0; i < 8; i++)
	{
		if (!userdefault->getStringForKey(_global->userInformation->getUserCaveFileNameKey(i).c_str()).size())
		{
			_global->userInformation->setUserCaveFileName(i, "未命名存档");
		}
		else
		{
			_global->userInformation->setUserCaveFileName(i, userdefault->getStringForKey(_global->userInformation->getUserCaveFileNameKey(i).c_str()));
		}
	}

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
	
	/* 是否高帧率 */
	switch (userdefault->getBoolForKey("SHOWHIGHFPS"))
	{
	case true:
		_director->setAnimationInterval(1.0f / 60);
		_global->userInformation->setIsSelectHighFPS(cocos2d::ui::CheckBox::EventType::SELECTED);
		_global->userInformation->setFps(60);
		break;
	case false:
		_director->setAnimationInterval(1.0f / 45);
		_global->userInformation->setIsSelectHighFPS(cocos2d::ui::CheckBox::EventType::UNSELECTED);
		_global->userInformation->setFps(45);
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

	/* 是否隐藏鼠标 */
	switch (userdefault->getBoolForKey("CURSORHIDE"))
	{
	case true:
		_global->userInformation->setIsSelectCursorNotHide(cocos2d::ui::CheckBox::EventType::SELECTED);
		break;
	case false:
		_global->userInformation->setIsSelectCursorNotHide(cocos2d::ui::CheckBox::EventType::UNSELECTED);
		break;
	}
}

void LoadingScene::showLoadingBackGround()
{
	/* 播放音乐 */
	_global->changeBgMusic("mainmusic", true);
	
	/* 获取窗口大小 */
	auto const size = Director::getInstance()->getWinSize();

	/* 创建精灵 */
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("resources/images/LoadingScene/LoadingScene.plist");
	_sprite[0] = Sprite::createWithSpriteFrameName("cocos2dx_Logo.png");
	_sprite[1] = Sprite::createWithSpriteFrameName("PopCap_Logo.png");
	_sprite[7] = Sprite::create("resources/text/txt/About.txt");
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
	_sprite[7]->setVisible(false);
	_sprite[2]->setVisible(false);

	/* 设置精灵动作 */
	_sprite[0]->runAction(Sequence::create(FadeIn::create(2.0f), FadeOut::create(2.0f), CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 1)), NULL));
}

void LoadingScene::runLoGoCallBack(Node* node, const int &ID)
{
	switch (ID)
	{
	case 1:
		this->removeChildByName("0"); /* 从场景中移除名字为0的孩子 */
		_sprite[1]->setVisible(true);  /* 设置精灵1可见 */
		_sprite[1]->runAction(Sequence::create(FadeIn::create(2.0f), FadeOut::create(2.0f), CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 5)), NULL));
		break;
	case 2:
		this->removeChildByName("7"); /* 从场景中移除名字为7的孩子 */
		_sprite[2]->setVisible(true);  /* 设置精灵2可见 */
		_sprite[2]->runAction(Sequence::create(FadeIn::create(2.0f),CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 3)), NULL));
		break;
	case 3:
		this->showTileAndLoadingBar(); /* 展示标题和进度条 */
		break;
	case 4:
		this->beginLoadingImageAndMusic(); /* 开始加载图片和音乐 */
		break;
	case 5:
		this->removeChildByName("1"); /* 从场景中移除名字为1的孩子 */
		_sprite[7]->setVisible(true);  /* 设置精灵7可见 */
		_sprite[7]->runAction(Sequence::create(FadeIn::create(2.0f), FadeOut::create(4.0f), CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 2)), NULL));
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
	_sprite[5]->setPosition(Vec2(10, 95));
	_sprite[6]->setPosition(Vec2(-SpriteSize.width, 0));
	clippingNode->setPosition(Vec2(size.width / 2 + 10, 1100));

	/* 为精灵设置大小 */
	_sprite[3]->setScaleX(1.25f);
	_sprite[4]->setScale(2.0f);
	_sprite[6]->setScale(2.0f);

	/* 让精灵运动起来 */
	_sprite[4]->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(size.width / 2, 150)),CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 4)), NULL));
	_sprite[6]->runAction(RepeatForever::create(Sequence::create(MoveTo::create(1.0f, Vec2(SpriteSize.width, 0)), DelayTime::create(2.0f),
		CallFunc::create([=]()
			{
				_sprite[6]->setPosition(Vec2(-SpriteSize.width, 0));
			}), nullptr)));
    clippingNode->runAction(MoveTo::create(0.5f, Vec2(size.width / 2, 900)));

	/* 加到场景中 */
	this->addChild(_sprite[4]);
	this->addChild(clippingNode);
	_sprite[4]->addChild(_sprite[5],1);
	
	/* 设置裁剪节点的基本属性 */
	clippingNode->setAlphaThreshold(0.01f);    //设置alpha闸值
	clippingNode->setContentSize(SpriteSize);  //设置尺寸大小
	clippingNode->setStencil(_sprite[3]);      //设置模板stencil
	clippingNode->addChild(_sprite[3]);        //先添加标题,会完全显示出来,因为跟模板一样大小
	clippingNode->addChild(_sprite[6]);        //会被裁减

	/* 创建菜单标签 */
	_label = MenuItemLabel::create(Label::createWithTTF("加载中......", GAME_FONT_NAME_1, 20), CC_CALLBACK_1(LoadingScene::beginGameCallBack, this));
	_label->setColor(Color3B::YELLOW);
	_label->setEnabled(false);
	
	/* 创建菜单 */
	auto menu = Menu::create(_label, NULL);
	menu->setPosition(150, 55);
	_sprite[4]->addChild(menu);
}

void LoadingScene::beginLoadingImageAndMusic()
{
	/* 创建进度条 */
	_loadingBar = LoadingBar::create();
	_loadingBar->loadTexture("LoadBar_grass.png", Widget::TextureResType::PLIST);
	_loadingBar->setDirection(LoadingBar::Direction::LEFT); /* 设置加载方向 */
	_loadingBar->setPosition(Vec2(150, 70));
	_sprite[4]->addChild(_loadingBar);

	/* 定时器 */
	scheduleUpdate();

	loadingText();         /* 加载文本 */
	loadingMusic();        /* 加载音乐 */
	loadingAnimation();    /* 加载动画 */
	loadingImage();        /* 加载图片 */	
	throwException();      /* 抛出异常 */
}

void LoadingScene::update(float Time)
{
	if (_loadingPrecent <= 100) 
	{
		_loadingBar->setPercent(_loadingPrecent);              /* 设置加载进度 */
		_sprite[5]->setScale(1 - _loadingPrecent / 170);       /* 设置精灵大小 */
		_sprite[5]->setRotation(9 * _loadingPrecent);          /* 设置精旋转度数 */
		_sprite[5]->setPosition(Vec2(10 + 290 / 100.0 * _loadingPrecent, 100 - _sprite[5]->getContentSize().height / 400 * _loadingPrecent));

		if (_loadingPrecent >= 20)  showLoadingBarFlower(0);
		if (_loadingPrecent >= 40)  showLoadingBarFlower(1);
		if (_loadingPrecent >= 60)  showLoadingBarFlower(2);
		if (_loadingPrecent >= 80)  showLoadingBarFlower(3);
		if (_loadingPrecent >= 100) showLoadingBarFlower(4);

	}
	if(_loadingPrecent >= 100) /* 如果加载完成 */
	{
		_label->setString(_global->userInformation->getGameText().find("点击开始")->second);  /* 重新设置标签文字内容 */
		auto action = TintBy::create(0.5f, 0, 255, 255);
		_label->runAction(RepeatForever::create(Sequence::create(action,action->reverse(), nullptr)));
		_sprite[5]->setVisible(false);  /* 设置精灵5为不可见 */
		_label->setEnabled(true);       /* 设置触摸可行 */
	}
}

void LoadingScene::showLoadingBarFlower(const int &ID)
{
	srand(unsigned(time(nullptr)));
	if (!_flowerVisible[ID]) /* 如果花的ID是不可见 */
	{
		/* 创建花 */
		auto flower = Sprite::createWithSpriteFrameName("sprout_petal.png");
		flower->setScale(0.1f);
		flower->setPosition(Vec2(20 + (55 + rand() % 10)* ID, 85 + rand() % 10));
		flower->runAction(ScaleTo::create(0.2f, 0.5f));
		_sprite[4]->addChild(flower);
		if (ID == 4) /* 如果ID==4 创建僵尸头*/
		{
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("loadingbar_zombie")->second, false), _global->userInformation->getSoundEffectVolume());
			flower->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("ZombieHead1.png"));
		}
		else  /* 否则创建花 */
		{
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("loadingbar_flower")->second, false), _global->userInformation->getSoundEffectVolume());
			flower->setRotation(rand() % 180);
		}
		
		_flowerVisible[ID] = true; /* 花ID设置为可见 */
	}
}

void LoadingScene::calculateFileNumbers()
{
#if MYDEBUG
	/* 文件总数 = 文本数 + 图片数 + 音乐数 + 动画数 */
	_allFileNumbers =
		openResourcesPath(_global->userInformation->getTextPath(), "resources/Text/TextPath.reanim.compiled", true) +
		openResourcesPath(_global->userInformation->getImagePath(), "resources/Text/ImagePath.reanim.compiled", true) +
		openResourcesPath(_global->userInformation->getMusicPath(), "resources/Text/MusicPath.reanim.compiled", true) +
		openResourcesPath(_global->userInformation->getAnimationPath(), "resources/Text/AnimationPath.reanim.compiled", true);
#else
	_allFileNumbers =
		openResourcesPath(_global->userInformation->getTextPath(), "resources/Text/TextPath.xml", false) +
		openResourcesPath(_global->userInformation->getImagePath(), "resources/Text/ImagePath.xml", false) +
		openResourcesPath(_global->userInformation->getMusicPath(), "resources/Text/MusicPath.xml", false) +
		openResourcesPath(_global->userInformation->getAnimationPath(), "resources/Text/AnimationPath.xml", false);
#endif
}

void LoadingScene::setRunFirstTime()
{
	time_t tt;
	struct tm* nowtime;
	time(&tt);
	nowtime = localtime(&tt);

	if (UserDefault::getInstance()->getStringForKey("FIRSTRUNTIME").size() == 0)
	{
		UserDefault::getInstance()->setStringForKey("FIRSTRUNTIME", to_string(nowtime->tm_year + 1900) + "年 " +
			to_string(nowtime->tm_mon) + "月 " + to_string(nowtime->tm_mday) + "日 星期" + to_string(nowtime->tm_wday) + " " +
			to_string(nowtime->tm_hour) + "时 " + to_string(nowtime->tm_min) + "分 " + to_string(nowtime->tm_sec) + "秒");
	}

	UserDefault::getInstance()->setIntegerForKey("BEGINDAY", nowtime->tm_mday);
	UserDefault::getInstance()->setIntegerForKey("BEGINHOUR", nowtime->tm_hour);
	UserDefault::getInstance()->setIntegerForKey("BEGINMIN", nowtime->tm_min);
	UserDefault::getInstance()->setIntegerForKey("BEGINSEC", nowtime->tm_sec);
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
#if CC_TARGET_PLATFORM ==CC_PLATFORM_WIN32
	this->runAction(Sequence::create(DelayTime::create(10.f), CallFunc::create([=]()
		{
			try
			{
				if (_loadFileNumbers < _allFileNumbers)
				{
					wstring str = L"糟糕！发生了一些错误，部分资源文件加载失败！请重试！";
					AudioEngine::play2d(_global->userInformation->getMusicPath().find("buzzer")->second);
					throw str;
				}
			}
			catch (wstring str)
			{
				if (MessageBoxW(_director->getOpenGLView()->getWin32Window(), str.c_str(), L"资源加载异常", MB_OK) == IDOK)
				{
					Director::getInstance()->end();
				}
			}
		}), nullptr));
#endif
}

void LoadingScene::loadingText()
{
	/* 循环加载文本 */
	for (auto& i : _global->userInformation->getTextPath())
	{
		ValueVector Text = _files->getValueVectorFromFile(i.second);
		for (auto& file : Text)
		{
			auto file_map = file.asValueMap();
			_global->userInformation->getGameText().insert(pair<string, string>(file_map.at("id").asString(), file_map.at("info").asString()));
		}
		/* 加载回调 */
		this->loadingTextCallBack();
	}
}

void LoadingScene::loadingImage()
{
    /* 循环加载图片 */
	for (auto& i : _global->userInformation->getImagePath())
	{
		_director->getTextureCache()->addImageAsync(i.second + "pvr.ccz", [=](Texture2D* texture)
			{
				SpriteFrameCache::getInstance()->addSpriteFramesWithFile(i.second + "plist", texture);
				_loadFileNumbers++;     /* 文件数加一 */
				_loadingPrecent = ((_loadFileNumbers * 1.0f) / _allFileNumbers) * 100;  /* 计算加载的百分比 */
			});
	}
}

void LoadingScene::loadingMusic()
{
	/* 循环加载音乐 */
	for (auto& i : _global->userInformation->getMusicPath())
	{
		AudioEngine::preload(i.second, [=](bool isSucceed)
			{
				if (isSucceed)/* 如果加载成功 */
				{
					_loadFileNumbers++;     /* 文件数加一 */
					_loadingPrecent = ((_loadFileNumbers * 1.0f) / _allFileNumbers) * 100;  /* 计算加载的百分比 */
				}
			});
	}
}

void LoadingScene::loadingAnimation()
{
	/* 循环加载动画 */
	for (auto& i : _global->userInformation->getAnimationPath())
	{
		/* 临时存储文件名字 */
		char JsonName[100], AtlasName[100];

		/* 转换 */
		snprintf(JsonName, 100, "resources/Animations/Json/%s.json", (i.second).c_str());
		snprintf(AtlasName, 100, "resources/Animations/Atlas/%s.atlas", (i.second).c_str());

		/* 加载 */
		spSkeletonJson* json = spSkeletonJson_createWithLoader((spAttachmentLoader*)Cocos2dAttachmentLoader_create(spAtlas_createFromFile(AtlasName, nullptr)));
		auto skeletonData = spSkeletonJson_readSkeletonDataFile(json, JsonName);
		spSkeletonJson_dispose(json);

		/* 把加载到的动画放入map中*/
		_global->userInformation->getAnimationData().insert(pair<string, spSkeletonData*>(i.second, skeletonData));

		/* 进行回调 */
		this->loadingAnimationCallBack();
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
	AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("buttonclick")->second), _global->userInformation->getSoundEffectVolume());
	_director->replaceScene(MainMenu::createScene()); /* 切换场景 */
}