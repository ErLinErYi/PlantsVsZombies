﻿/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#include "LZAGSPauseQuitLayer.h"
#include "LZAGSGameEndLayer.h"
#include "LZAGameScene.h"
#include "LZAGSData.h"

#include "Scenes/GameScenes/BigMap/GameScene/LZBigMapGameScene.h"

#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSMirrorSelectPlantsScene.h"
#include "Scenes/GameScenes/Adventure/WorldScene/LZModernWorld.h"

#include "spine/spine-cocos2dx.h"
#include "Based/LZUserData.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZDefine.h"

using namespace spine;

string GSPauseQuitLayer::_layerName[] = 
{ 
	"backgroundLayer","buttonLayer","animationLayer",
	"controlLayer","informationLayer","goodsLayer","gameTimerLayer"
};

bool GSPauseQuitLayer::_isPause = false;

GSPauseQuitLayer::GSPauseQuitLayer() :
  _promptLayer(nullptr)
, _levelName(_global->userInformation->getCurrentCaveFileLevelWorldName())
{
}

GSPauseQuitLayer::~GSPauseQuitLayer()
{
	_isPause = false;
}

void GSPauseQuitLayer::pauseLayer()
{
	const auto director = Director::getInstance()->getRunningScene();

	for (const auto &name : _layerName)
	{
		if (director->getChildByName(name))
			director->getChildByName(name)->onExit();
	}
	PlayMusic::stopMusic();
	_isPause = true;
}

void GSPauseQuitLayer::resumeLayer()
{
	const auto director = Director::getInstance()->getRunningScene();

	for (const auto& name : _layerName)
	{
		if (director->getChildByName(name))
			director->getChildByName(name)->onEnter();
	}
	PlayMusic::resumeMusic();
	_isPause = false;
}

void GSPauseQuitLayer::setPause(const bool pause)
{
	_isPause = pause;
}

bool GSPauseQuitLayer::getIsPause()
{
	return _isPause;
}

bool GSPauseQuitLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createDialog();
	
	return true;
}

void GSPauseQuitLayer::createDialog()
{
	_option = Sprite::createWithSpriteFrameName("LevelObjiectivesBg.png");
	_option->setPosition(_director->getWinSize() / 2);
	_option->setScale(0.9f);
	this->addChild(_option);

	auto PauseAnimation= SkeletonAnimation::createWithData(
		_global->userInformation->getAnimationData().find("PauseAnimation")->second);
	PauseAnimation->setAnimation(0, "animation", true);
	PauseAnimation->setPosition(Vec2(530, 650));
	_option->addChild(PauseAnimation);

	/* 触摸移动监听 */
	_touchListener = createTouchtListener(_option);

	/* 创建滑动条 */
	auto musicslider = createSlider(Vec2(600, 520), Vec2(150, 520), 
		GAME_TEXT("音乐"), OptionScene_Slider::音乐,
		Sprite::createWithSpriteFrameName("bgFile.png"),
		Sprite::createWithSpriteFrameName("progressFile.png"),
		Sprite::createWithSpriteFrameName("thumbFile.png"),
		nullptr,
		true);
	auto SoundEffectslider = createSlider(Vec2(600, 450), Vec2(150, 450), 
		GAME_TEXT("音效"), OptionScene_Slider::音效,
		Sprite::createWithSpriteFrameName("bgFile.png"),
		Sprite::createWithSpriteFrameName("progressFile.png"),
		Sprite::createWithSpriteFrameName("thumbFile.png"),
		nullptr,
		true);

	musicslider->setScale(1.2f);
	SoundEffectslider->setScale(1.2f);

	/* 创建复选框 */
	auto check   = createCheckBox(Vec2(350, 380), Vec2(150, 380), GAME_TEXT("动态难度"), OptionScene_CheckBox::动态难度, "CheckBox2", "CheckBox", true);
	auto check1  = createCheckBox(Vec2(800, 380), Vec2(600, 380), GAME_TEXT("全屏"), OptionScene_CheckBox::全屏, "CheckBox2", "CheckBox", true);
	auto check2  = createCheckBox(Vec2(350, 310), Vec2(150, 310), GAME_TEXT("高帧率"), OptionScene_CheckBox::高帧率, "CheckBox2", "CheckBox", true);
	auto check3  = createCheckBox(Vec2(800, 310), Vec2(600, 310), GAME_TEXT("鼠标显示"), OptionScene_CheckBox::鼠标隐藏, "CheckBox2", "CheckBox", true);
	auto check4  = createCheckBox(Vec2(350, 240), Vec2(150, 240), GAME_TEXT("拉伸显示"), OptionScene_CheckBox::拉伸显示, "CheckBox2", "CheckBox", true);
	auto check5  = createCheckBox(Vec2(800, 240), Vec2(600, 240), GAME_TEXT("垂直同步"), OptionScene_CheckBox::垂直同步, "CheckBox2", "CheckBox", true);

	check->setScale(0.6f);
	check1->setScale(0.6f);
	check2->setScale(0.6f);
	check3->setScale(0.6f);
	check4->setScale(0.6f);
	check5->setScale(0.6f);

	/* 创建按钮 */
	createButton(Vec2(210, 170), GAME_TEXT("查看图鉴"), PauseQuitLayer_Button::查看图鉴);
	createButton(Vec2(520, 170), GAME_TEXT("重新开始"), PauseQuitLayer_Button::从新开始);
	createButton(Vec2(830, 170), GAME_TEXT("返回游戏"), PauseQuitLayer_Button::返回游戏);
	createButton(Vec2(365, 70), GAME_TEXT("按键说明"), PauseQuitLayer_Button::按键说明);
	createButton(Vec2(665, 70), GAME_TEXT("退出并保存"), PauseQuitLayer_Button::退出游戏);
}

void GSPauseQuitLayer::createButton(const Vec2& vec2, const std::string name, PauseQuitLayer_Button button_type)
{
	/* 创建返回主菜单按钮 */
	auto button = ui::Button::create("ButtonNew2.png", "ButtonNew.png", "",cocos2d::ui::Widget::TextureResType::PLIST);
	auto label = Label::createWithTTF(name, GAME_FONT_NAME_1, GAME_TEXT_SIZE("退出并保存"));
	label->enableShadow(Color4B(0, 0, 0, 200));//设置阴影
	label->setScale(2.0f);
	button->setTitleLabel(label);
	button->setTitleColor(Color3B::WHITE);
	button->setPosition(vec2);
	button->setScale(0.5f);
	_option->addChild(button);

	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("gravebutton");
				break;
			case ui::Widget::TouchEventType::ENDED:
				//button->setTouchEnabled(false);
				switch (button_type)
				{
				case PauseQuitLayer_Button::查看图鉴: openHandBook();   break;
				case PauseQuitLayer_Button::从新开始: setRestart();     break;
				case PauseQuitLayer_Button::退出游戏: setQuitGame();    break;
				case PauseQuitLayer_Button::按键说明: keyDescription(); break;
				case PauseQuitLayer_Button::返回游戏: returnGame();     break;
				case PauseQuitLayer_Button::重置游戏: remakeGame();     break;
				default: break;
				}
			}
		});
}

void GSPauseQuitLayer::showPrompt()
{
	_promptLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	createShieldLayer(_promptLayer);
	this->addChild(_promptLayer);

	auto prompt = Sprite::createWithSpriteFrameName("Prompt.png");
	prompt->setPosition(_director->getWinSize() / 2.0f);
	prompt->setOpacity(200);
	_promptLayer->addChild(prompt);

	auto close = ui::Button::create("CloseDown.png", "Close.png", "",cocos2d::ui::Widget::TextureResType::PLIST);
	close->setPosition(Vec2(1510, 422));
	close->setScale(0.6f);
	close->setScaleY(0.58f);
	prompt->addChild(close);
	close->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap");
				break;
			case ui::Widget::TouchEventType::ENDED:
				_promptLayer->removeFromParent();
				break;
			}
		});

	auto text = Text::create();
	text->setFontName(GAME_FONT_NAME_1);
	text->setFontSize(GAME_TEXT_SIZE("按键信息"));
	text->setColor(Color3B::WHITE);
	text->setTextAreaSize(Size(1800, 300));
	text->setTextHorizontalAlignment(TextHAlignment::LEFT);
	text->setTextVerticalAlignment(TextVAlignment::TOP);
	text->setPosition(Vec2(960, 150));
	text->setString(GAME_TEXT("按键信息"));
	_promptLayer->addChild(text);
}

void GSPauseQuitLayer::openHandBook()
{
	Application::getInstance()->openURL("https://gitlz.gitee.io/lz");
}

void GSPauseQuitLayer::setRestart()
{
	CREATEDIALOG(
		_director->getScheduler()->setTimeScale(1.0f);
	    UserData::getInstance()->caveUserData(const_cast<char*>("BREAKTHROUGH"), ++_global->userInformation->getBreakThroughNumbers());
	    GSGameEndLayer::judgeBreakThroughAboutJumpLevel();

		_global->userInformation->setDynamicDifficultyValue(_global->userInformation->getDynamicDifficultyValue() - 5 - rand() % 5);
		UserData::getInstance()->caveUserData(const_cast<char*>("DYNAMICDIFFICULTYVALUE"), _global->userInformation->getDynamicDifficultyValue());

	    _director->replaceScene(TransitionFade::create(1.0f, SelectPlantsScene::create()));

	    UserData::getInstance()->createNewLevelDataDocument();
	    UserData::getInstance()->removeLevelData(_levelName);
	)
}

void GSPauseQuitLayer::setQuitGame()
{
	CREATEDIALOG(
	    _director->getScheduler()->setTimeScale(1.0f);
	    UserData::getInstance()->caveUserData(const_cast<char*>("BREAKTHROUGH"), ++_global->userInformation->getBreakThroughNumbers());
	    GSGameEndLayer::judgeBreakThroughAboutJumpLevel();

	    if (GameScene::isRunGameScene) /* 如果运行了游戏场景则保存 */
	    {
		    UserData::getInstance()->createNewLevelDataDocument();
			UserData::getInstance()->caveLevelData(_levelName, true);
	    }

	    popSceneAnimation();
	)
}

void GSPauseQuitLayer::keyDescription()
{
	showPrompt();
}

void GSPauseQuitLayer::returnGame()
{
	GameScene::isRunGameScene = true;
	resumeLayer();
	this->removeFromParent();
}

void GSPauseQuitLayer::remakeGame()
{
}

void GSPauseQuitLayer::popSceneAnimation()
{
	auto layer = LayerColor::create(Color4B(0, 0, 0, 0));
	this->addChild(layer);
	layer->runAction(Sequence::create(FadeIn::create(0.5f),
		CallFunc::create([=]()
			{
				ModernWorld::setPopEnter(true);
				layer->removeFromParent();
				_director->popScene();
			}), nullptr));
}