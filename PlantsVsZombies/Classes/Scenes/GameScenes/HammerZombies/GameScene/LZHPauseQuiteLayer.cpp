/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */

#include "LZHPauseQuiteLayer.h"
#include "LZHammerZombiesScene.h"
#include "Scenes/MainMenuScene/LZMainMenu.h"
#include "Based/LZPlayMusic.h"

HPauseQuitLayer::HPauseQuitLayer()
{
}

HPauseQuitLayer::~HPauseQuitLayer()
{
}

bool HPauseQuitLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createDialog();

	return true;
}

void HPauseQuitLayer::createDialog()
{
	_option = Sprite::createWithSpriteFrameName("LevelObjiectivesBg.png");
	_option->setPosition(_director->getWinSize() / 2);
	_option->setScale(0.9f);
	this->addChild(_option);

	auto PauseAnimation = SkeletonAnimation::createWithData(
		_global->userInformation->getAnimationData().find("PauseAnimation")->second);
	PauseAnimation->setAnimation(0, "animation", true);
	PauseAnimation->setPosition(Vec2(530, 650));
	_option->addChild(PauseAnimation);

	/* 触摸移动监听 */
	_touchListener = createTouchtListener(_option);

	/* 创建滑动条 */
	auto musicslider = createSlider(Vec2(600, 520), Vec2(150, 520),
		_global->userInformation->getGameText().find("音乐")->second->text, OptionScene_Slider::音乐,
		Sprite::createWithSpriteFrameName("bgFile.png"),
		Sprite::createWithSpriteFrameName("progressFile.png"),
		Sprite::createWithSpriteFrameName("thumbFile.png"),
		nullptr,
		true);
	auto SoundEffectslider = createSlider(Vec2(600, 450), Vec2(150, 450),
		_global->userInformation->getGameText().find("音效")->second->text, OptionScene_Slider::音效,
		Sprite::createWithSpriteFrameName("bgFile.png"),
		Sprite::createWithSpriteFrameName("progressFile.png"),
		Sprite::createWithSpriteFrameName("thumbFile.png"),
		nullptr,
		true);

	musicslider->setScale(1.2f);
	SoundEffectslider->setScale(1.2f);

	/* 创建复选框 */
	auto check = createCheckBox(Vec2(350, 380), Vec2(150, 380), _global->userInformation->getGameText().find("信息")->second->text, OptionScene_CheckBox::显示信息, "CheckBox2", "CheckBox", true);
	auto check1 = createCheckBox(Vec2(800, 380), Vec2(600, 380), _global->userInformation->getGameText().find("全屏")->second->text, OptionScene_CheckBox::全屏, "CheckBox2", "CheckBox", true);
	auto check2 = createCheckBox(Vec2(350, 310), Vec2(150, 310), _global->userInformation->getGameText().find("高帧率")->second->text, OptionScene_CheckBox::高帧率, "CheckBox2", "CheckBox", true);
	auto check3 = createCheckBox(Vec2(800, 310), Vec2(600, 310), _global->userInformation->getGameText().find("鼠标显示")->second->text, OptionScene_CheckBox::鼠标隐藏, "CheckBox2", "CheckBox", true);
	auto check4 = createCheckBox(Vec2(350, 240), Vec2(150, 240), _global->userInformation->getGameText().find("拉伸显示")->second->text, OptionScene_CheckBox::拉伸显示, "CheckBox2", "CheckBox", true);
	auto check5 = createCheckBox(Vec2(800, 240), Vec2(600, 240), _global->userInformation->getGameText().find("垂直同步")->second->text, OptionScene_CheckBox::垂直同步, "CheckBox2", "CheckBox", true);

	check->setScale(0.6f);
	check1->setScale(0.6f);
	check2->setScale(0.6f);
	check3->setScale(0.6f);
	check4->setScale(0.6f);
	check5->setScale(0.6f);

	/* 创建按钮 */
	createButton(Vec2(210, 170), _global->userInformation->getGameText().find("查看图鉴")->second->text, PauseQuitLayer_Button::查看图鉴);
	createButton(Vec2(520, 170), _global->userInformation->getGameText().find("重新开始")->second->text, PauseQuitLayer_Button::从新开始);
	createButton(Vec2(830, 170), _global->userInformation->getGameText().find("返回游戏")->second->text, PauseQuitLayer_Button::返回游戏);
	createButton(Vec2(365, 70), _global->userInformation->getGameText().find("按键说明")->second->text, PauseQuitLayer_Button::按键说明);
	createButton(Vec2(665, 70), _global->userInformation->getGameText().find("退出")->second->text, PauseQuitLayer_Button::退出游戏);
}

void HPauseQuitLayer::setRestart()
{
	_director->getScheduler()->setTimeScale(1.0f);
	_director->replaceScene(TransitionFade::create(1.5f, HammerZombiesScene::create()));
}

void HPauseQuitLayer::setQuitGame()
{
	_director->getScheduler()->setTimeScale(1.0f);
	_director->replaceScene(TransitionFade::create(0.5f, MainMenu::create()));
}
