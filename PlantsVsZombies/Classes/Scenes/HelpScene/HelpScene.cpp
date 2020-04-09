/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.1
 *Email: 2117610943@qq.com
 */

#include "HelpScene.h"
#include "Scenes/MainMenuScene/MainMenu.h"
#include "ui/CocosGUI.h"
#include "Scenes/EasterEggsScene/GameEasterEggs.h"
#include "Scenes/WorldScene/SelectWorldScene.h"

string INFORMATION_TEXT = { "\
		此游戏全部由本人自己制作完成。此版本为测试版(具有一定的可玩性)，今后会不定时更新（不断完善）。目前我己经把\
该项目的源码开源发到GitHub。\n\
		游戏大部分的素材来源于原版游戏素材，少部分搜集于网络，以及自己制作。\n\
		（！！！重要）此游戏为同人游戏而且仅供学习交流使用，由于游戏资源可能存在侵权的问题，所以请勿用于商业用途，否则后果自负。\n\
		目前有7种僵尸和13种植物，植物和僵尸的动画都是本人做的，由于做动画的能力有限，有些僵尸和植物动画不能实现，\
如果可以做动画并且愿意帮助我的人可以私聊我（动画是用spine软件制作的骨骼动画）。如果发现程序有什么问题或者对游戏有\
什么建议可以发送到我的qq：2117610943\n\
       点击主菜单的下面三个按钮可以跳转到下载网址（百度网盘提取码3vzm)\n" };

Scene* HelpScene::createHelpScene()
{
	return HelpScene::create();
}

bool HelpScene::init()
{
	if (!Scene::init()) return false;

	AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("paper")->second), _global->userInformation->getSoundEffectVolume());

	createBackground();
	createText();
	createButton();
	
	return true;
}

void HelpScene::createBackground()
{
	SelectWorldScene::createGalaxy(this);
}

void HelpScene::createText()
{
	/* 创建文字背景 */
	auto note = Sprite::createWithSpriteFrameName("ZombieNote.png");
	note->setScale(1.7f);
	note->setScaleX(2.2f);
	note->setPosition(Vec2(_size.width / 2 + 50, _size.height / 2));
	this->addChild(note);

	addScrollView();

	FileUtils::getInstance()->getStringFromFile("resources/Text/history.reanim.compiled", [=](string history)
		{
			auto helptext = Label::createWithTTF(INFORMATION_TEXT + (history.empty() ? "\t\t\t\t\t\t\t\t\t\t\t\t\t文本加载失败！" : history), GAME_FONT_NAME_1, 35);
			helptext->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
			helptext->setPosition(Vec2(600, _textScrollView->getInnerContainerSize().height));
			helptext->setColor(Color3B::BLACK);
			helptext->setMaxLineWidth(1100);
			_textScrollView->addChild(helptext);
		});

	addMouseEvent();
}

void HelpScene::createGithubUrl()
{
	auto github = Button::create();
	github->setTitleFontName(GAME_FONT_NAME_1);
	github->setTitleFontSize(35);
	github->setTitleText("点击此处打开github项目");
	github->setPosition(Vec2(1220, 740));
	github->setTitleColor(Color3B(0, 255, 255));
	github->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap2")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case Widget::TouchEventType::ENDED:
				Application::getInstance()->openURL("https://github.com/ErLinErYi/PlantsVsZombies");
				break;
			}
		});
	this->addChild(github);
	
	auto draw = DrawNode::create();
	draw->drawLine(Vec2(0, 5), Vec2(github->getContentSize().width, 5), Color4F(0, 1, 1, 1));
	draw->setLineWidth(2);
	github->addChild(draw);
}

void HelpScene::createButton()
{
	/* 创建按钮 */
	auto button = Button::create("SeedChooser_Button2.png", "SeedChooser_Button2_Glow.png", "", TextureResType::PLIST);
	button->setTitleText(_global->userInformation->getGameText().find("主菜单")->second);
	button->setTitleColor(Color3B(0, 255, 255));
	button->setTitleFontSize(18);
	button->setScale(2.0f);
	button->setPosition(Vec2(_size.width / 2, 100));
	this->addChild(button);
	button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap2")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case Widget::TouchEventType::ENDED:
				Director::getInstance()->replaceScene(MainMenu::createScene());
				break;
			}
		});

}

void HelpScene::addScrollView()
{
	_textScrollView = ui::ScrollView::create();
	_textScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	_textScrollView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_textScrollView->setContentSize(Size(1280.0f, 640.0f));
	_textScrollView->setInnerContainerSize(Size(1280, 6500));
	_textScrollView->setPosition(_size / 2.0f);
	_textScrollView->setBounceEnabled(true);
	_textScrollView->setScrollBarPositionFromCorner(Vec2(20, 0));
	_textScrollView->setScrollBarWidth(10);
	_textScrollView->setScrollBarColor(Color3B::BLACK);
	this->addChild(_textScrollView);
}

void HelpScene::addMouseEvent()
{
	/* 鼠标滑动监听 */
	auto mouse = EventListenerMouse::create();
	mouse->onMouseScroll = [=](Event* event)
	{
		auto mouseEvent = static_cast<EventMouse*>(event);
		float movex = mouseEvent->getScrollY() * 5;

		auto minOffset = 0.f;
		auto maxOffset = 100.f;

		auto offset = _textScrollView->getScrolledPercentVertical();
		offset += movex;

		if (offset < minOffset)
		{
			offset = minOffset;
		}
		else if (offset > maxOffset)
		{
			offset = maxOffset;
		}
		_textScrollView->scrollToPercentVertical(offset, 0.5f, true);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse, _textScrollView);
}
