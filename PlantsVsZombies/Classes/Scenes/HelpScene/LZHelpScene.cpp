/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.1
 *Email: 2117610943@qq.com
 */
#include "ui/CocosGUI.h"

#include "LZHelpScene.h"
#include "Scenes/MainMenuScene/LZMainMenu.h"
#include "Scenes/EasterEggsScene/LZGameEasterEggs.h"
#include "Scenes/SelectWorldScene/LZSelectWorldScene.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZMouseEventControl.h"

string INFORMATION_TEXT = { "\
        此游戏全部由哔哩哔哩\"尔领尔亿\"（LZ）独立制作完成。此游戏与当前可以见到的众多改版不同。此程序不是基于原版游戏的二次开发，而是从新从零开始制作。\
可以说此程序与原版程序没有任何关系。该程序使用Cocos2dx游戏开发框架以及C++语言制作完成。目前该程序的所有源代码已经开源到github以及gitee上。\n\
        此游戏目前已经有多种游戏模式。有着不同种类的游戏关卡、有着超过几百关数量的游戏关卡、有着超大地图（10 * 18）的游戏模式、有着“锤僵尸”无尽版的游戏模式等等种类。\
此游戏使用的绝大部分图片、音乐、音效、字体等资源文件来源于原版一代、二代、以及三代的游戏资源，少部分收集于网络以及自己制作。\n\
	    此程序使用原版游戏素材所以不得用于商业用途，否则后果自负。当你运行此游戏或者使用其源代码时则认为默认同意此协议，否则请退出游戏并删除。\
项目地址可以直接在github或gitee上搜索关键词\"PlantsVsZombies\"或浏览器中输入以下地址：https://github.com/ErLinErYi/PlantsVsZombies \n\
		目前有 15 种僵尸和 24 种植物，由于做动画的能力有限，有些僵尸和植物动画不能实现或动画与原版存在很大偏差。\
（使用spine或者DragonBones制作的2D动画）目前已经有多种游戏模式和超过几百个游戏关卡。\nQQ:2117610943 \nQQ群:926707238 \n网址:https://gitlz.gitee.io/lz \n\n"
};

Scene* HelpScene::createHelpScene()
{
	return HelpScene::create();
}

bool HelpScene::init()
{
	if (!Scene::init()) return false;

	PlayMusic::playMusic("paper");
	
	createBackground();
	createParticle();
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

	FileUtils::getInstance()->getStringFromFile("history.pak", [=](string history)
		{
			auto helptext = Label::createWithTTF(INFORMATION_TEXT + (history.empty() ? "\t\t\t\t\t\t\t\t\t\t\t\t\t文本加载失败！" : history), GAME_FONT_NAME_1, 35);
			helptext->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
			helptext->setColor(Color3B::BLACK);
			helptext->setMaxLineWidth(1100);
			_textScrollView->setInnerContainerSize(helptext->getContentSize());
			_textScrollView->addChild(helptext);
			helptext->setPosition(Vec2(600, _textScrollView->getInnerContainerSize().height));
			helptext->setLineBreakWithoutSpace(true);
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
				PlayMusic::playMusic("tap2");
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
	button->setTitleText(_global->userInformation->getGameText().find("主菜单")->second->text);
	button->setTitleColor(Color3B(0, 255, 255));
	button->setTitleFontSize(_global->userInformation->getGameText().find("主菜单")->second->fontsize);
	button->setScale(2.0f);
	button->setPosition(Vec2(_size.width / 2, 100));
	this->addChild(button);
	button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap2");
				break;
			case Widget::TouchEventType::ENDED:
				Director::getInstance()->replaceScene(MainMenu::createScene());
				break;
			}
		});

}

void HelpScene::createParticle()
{
	/*auto particle = ParticleSystemQuad::create("resources/Text/Particle.plist");
	particle->setPosition(Vec2(0, -30));
	this->addChild(particle);*/
}

void HelpScene::addScrollView()
{
	_textScrollView = ui::ScrollView::create();
	_textScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	_textScrollView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_textScrollView->setContentSize(Size(1280.f, 570.f));
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

		MouseEventControl::mouseScrollControlListener(_textScrollView, movex, ui::ScrollView::Direction::VERTICAL);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse, _textScrollView);
}
