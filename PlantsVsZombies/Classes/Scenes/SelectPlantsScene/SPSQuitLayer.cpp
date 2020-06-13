/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "SPSQuitLayer.h"
#include "spine/spine-cocos2dx.h"
#include "Based/PlayMusic.h"

using namespace spine;

bool SPSQuitLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createShieldLayer(this);
	createDialog();

	return true;
}

void SPSQuitLayer::createDialog()
{
	_option = Sprite::createWithSpriteFrameName("LevelObjiectivesBg.png");
	_option->setPosition(_director->getWinSize() / 2);
	_option->setScale(0.9f);
	this->addChild(_option);

	auto PauseAnimation = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("PauseAnimation")->second);
	PauseAnimation->setAnimation(0, "animation", true);
	PauseAnimation->setPosition(Vec2(530, 650));
	_option->addChild(PauseAnimation);

	/* 触摸移动监听 */
	_touchListener = createTouchtListener(_option);

	/* 创建滑动条 */
	auto musicslider = createSlider(Vec2(600, 500), Vec2(150, 500), _global->userInformation->getGameText().find("音乐")->second, OptionScene_Slider::音乐,
		Sprite::createWithSpriteFrameName("bgFile.png"),
		Sprite::createWithSpriteFrameName("progressFile.png"),
		Sprite::createWithSpriteFrameName("thumbFile.png"),
		nullptr,
		true);
	auto SoundEffectslider = createSlider(Vec2(600, 410), Vec2(150, 410), _global->userInformation->getGameText().find("音效")->second, OptionScene_Slider::音效,
		Sprite::createWithSpriteFrameName("bgFile.png"),
		Sprite::createWithSpriteFrameName("progressFile.png"),
		Sprite::createWithSpriteFrameName("thumbFile.png"),
		nullptr,
		true);

	musicslider->setScale(1.2f);
	SoundEffectslider->setScale(1.2f);

	/* 创建复选框 */
	auto check = createCheckBox(Vec2(350, 340), Vec2(150, 340), _global->userInformation->getGameText().find("信息")->second, OptionScene_CheckBox::显示信息, "CheckBox2", "CheckBox", true);
	auto check1 = createCheckBox(Vec2(800, 340), Vec2(600, 340), _global->userInformation->getGameText().find("全屏")->second, OptionScene_CheckBox::全屏, "CheckBox2", "CheckBox", true);
	auto check2 = createCheckBox(Vec2(350, 265), Vec2(150, 265), _global->userInformation->getGameText().find("高帧率")->second, OptionScene_CheckBox::高帧率, "CheckBox2", "CheckBox", true);
	auto check3 = createCheckBox(Vec2(800, 265), Vec2(600, 265), _global->userInformation->getGameText().find("垂直同步")->second, OptionScene_CheckBox::垂直同步, "CheckBox2", "CheckBox", true);
	auto check4 = createCheckBox(Vec2(350, 190), Vec2(150, 190), _global->userInformation->getGameText().find("拉伸显示")->second, OptionScene_CheckBox::拉伸显示, "CheckBox2", "CheckBox", true);
	auto check5 = createCheckBox(Vec2(800, 190), Vec2(600, 190), _global->userInformation->getGameText().find("缓入动画")->second, OptionScene_CheckBox::缓入动画, "CheckBox2", "CheckBox", true);

	check->setScale(0.6f);
	check1->setScale(0.6f);
	check2->setScale(0.6f);
	check3->setScale(0.6f);
	check4->setScale(0.6f);
	check5->setScale(0.6f);

	/* 创建按钮 */
	GSPauseQuitLayer::createButton(Vec2(210, 100), _global->userInformation->getGameText().find("查看图鉴")->second, PauseQuitLayer_Button::查看图鉴);
	SPSQuitLayer::createButton(Vec2(830, 100), _global->userInformation->getGameText().find("退出游戏1")->second, PauseQuitLayer_Button::退出游戏);
	SPSQuitLayer::createButton(Vec2(520, 100), _global->userInformation->getGameText().find("返回游戏")->second, PauseQuitLayer_Button::返回游戏);
}

void SPSQuitLayer::createButton(const Vec2& vec2, const std::string name, PauseQuitLayer_Button button_type)
{
	/* 创建返回主菜单按钮 */
	auto button = ui::Button::create("ButtonNew2.png", "ButtonNew.png", "", TextureResType::PLIST);
	auto label = Label::createWithTTF(name, GAME_FONT_NAME_1, 35);
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
				switch (button_type)
				{
				case PauseQuitLayer_Button::返回游戏:
					this->removeFromParent();
					break;
				case PauseQuitLayer_Button::退出游戏:
					popSceneAnimation();
					break;
				default:
					break;
				}
			}
		});
}