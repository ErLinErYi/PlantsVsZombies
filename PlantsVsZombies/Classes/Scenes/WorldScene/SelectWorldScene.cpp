/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.15
 *Modify: 2020.1.28
 *Email: 2117610943@qq.com
 */

#include "World_1.h"
#include "MirrorWorld_1.h"
#include "SelectWorldScene.h"
#include "../MainMenuScene/MainMenu.h"

#include "Based/GlobalVariable.h"
#include "Based/UserData.h"
#include "Based/PlayMusic.h"
#include "AudioEngine.h"

SelectWorldScene::SelectWorldScene():
	_scrollView(nullptr), 
	_global(Global::getInstance())
{
	_selectWorldPosition = UserData::getInstance()->openDoubleUserData("SELECTWORLDPOSITION");
}

Scene* SelectWorldScene::createScene()
{
	return SelectWorldScene::create();
}

void SelectWorldScene::createMouseListener()
{
	/* 鼠标滑动监听 */
	auto mouse = EventListenerMouse::create();
	mouse->onMouseScroll = [&](Event* event)
	{
		auto mouseEvent = static_cast<EventMouse*>(event);
		float movex = mouseEvent->getScrollY() * 40;

		auto minOffset = 0.f;
		auto maxOffset = 100.f;

		auto offset = _scrollView->getScrolledPercentHorizontal();
		offset += movex;

		if (offset < minOffset)
		{
			offset = minOffset;
		}
		else if (offset > maxOffset)
		{
			offset = maxOffset;
		}
		_scrollView->scrollToPercentHorizontal(offset, 0.5f, true);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse, _scrollView);
}

bool SelectWorldScene::init()
{
	if (!Scene::init())return false;

	if (_global->userInformation->getMainToWorld())
	{
		showTimeTravelAnimation();
	}

	showBackground();
	createScrollView();
	showDifferentWorlds();
	createMouseListener();
	createGoBack();
	createSelectDifficulty();
	
	return true;
}

void SelectWorldScene::createGoBack()
{
	auto back = ui::Button::create("back.png", "back1.png", "", TextureResType::PLIST);
	back->setScale(0.7f);
	back->setAnchorPoint(Vec2(0, 1));
	back->setPosition(Vec2(0, 1080));
	this->addChild(back);

	back->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("gravebutton");
				break;
			case ui::Widget::TouchEventType::ENDED:
				Director::getInstance()->replaceScene(MainMenu::createScene());
				UserData::getInstance()->caveUserData("SELECTWORLDPOSITION",
					_scrollView->getScrolledPercentHorizontal());
				break;
			}
		});
}

void SelectWorldScene::createSelectDifficulty()
{
	_global->userInformation->setGameDifficulty(UserData::getInstance()->openIntUserData("DIFFICULTY"));
		
	auto checkbox = CheckBox::create();
	checkbox->loadTextureBackGround("CheckBox2.png", TextureResType::PLIST);
	checkbox->loadTextureFrontCross("CheckBox.png", TextureResType::PLIST);
	checkbox->setPosition(Vec2(100,1080));
	checkbox->setAnchorPoint(Vec2(0, 1));
	checkbox->setSelected(_global->userInformation->getGameDifficulty() ? true : false);
	this->addChild(checkbox);
	
	auto text = Text::create();
	text->setPosition(Vec2(47, -20));
	text->setTextColor(Color4B(0, 255, 255, 200));
	text->setFontName(GAME_FONT_NAME_1);
	text->setFontSize(30);
	text->setString(_global->userInformation->getGameDifficulty() ? "噩梦模式" : "简单模式");
	checkbox->addChild(text);

	checkbox->addEventListener([=](Ref* sender, CheckBox::EventType type)
		{
			switch (type)
			{
			case cocos2d::ui::CheckBox::EventType::SELECTED:
				_global->userInformation->setGameDifficulty(1);
				text->setString("噩梦模式");
				break;
			case cocos2d::ui::CheckBox::EventType::UNSELECTED:
				_global->userInformation->setGameDifficulty(0);
				text->setString("简单模式");
				break;
			}
			_global->userInformation->getUserSelectWorldData().at(
				_global->userInformation->getIsMirrorScene() ? 1 : 0)->isReadWoldInformation = false;
			UserData::getInstance()->caveUserData("DIFFICULTY", _global->userInformation->getGameDifficulty());
		});
}

void SelectWorldScene::showTimeTravelAnimation()
{
	/* 播放音乐 */
	PlayMusic::changeBgMusic("mainmusic2", true);
	
	auto layer = LayerColor::create(Color4B::BLACK);
	layer->setLocalZOrder(10);
	this->addChild(layer);

	auto clocks = PlayMusic::playMusic("Clocks",0);
	PlayMusic::setMusicVolume(clocks);
	
	auto TimeTraveller = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("TimeTraveller")->second);
	TimeTraveller->setAnimation(0, "animation", true);
	TimeTraveller->setContentSize(Size(1920, 1080));
	TimeTraveller->setPosition(Vec2(700, 598));
	TimeTraveller->setGlobalZOrder(1);
	TimeTraveller->setOpacity(0);
	this->addChild(TimeTraveller);

	TimeTraveller->runAction(Sequence::create(FadeIn::create(1.0f), DelayTime::create(2),
		CallFunc::create([layer]() {layer->removeFromParent(); }),FadeOut::create(1.0f),
		CallFunc::create([=]()
			{
				AudioEngine::stop(clocks);
				TimeTraveller->removeFromParent();
				_world[0]->setEnabled(true);
				_world[1]->setEnabled(true);
			}), nullptr));
}

void SelectWorldScene::showBackground()
{
	_backgroundSize = Director::getInstance()->getWinSize();

	createGalaxy(this);
}

void SelectWorldScene::createGalaxy(Node* node)
{
	auto layerColor = LayerColor::create(Color4B(30, 180, 190, 200));
	node->addChild(layerColor);

	auto rotate = Sprite::createWithSpriteFrameName("RunBackground.png");
	rotate->setScale(7.f);
	rotate->setPosition(Director::getInstance()->getWinSize() / 2.0f);
	node->addChild(rotate);
	rotate->runAction(RepeatForever::create(RotateBy::create(1.0f, -10)));

	auto worldBackgroundBlack = Sprite::createWithSpriteFrameName("WorldBackgroundBlack.png");
	auto worldBackgroundBlack1 = Sprite::createWithSpriteFrameName("WorldBackgroundBlack.png");
	worldBackgroundBlack->setAnchorPoint(Vec2(0, 1));
	worldBackgroundBlack->setPosition(Vec2(0, 1140));
	worldBackgroundBlack1->setAnchorPoint(Vec2(0, 0));
	worldBackgroundBlack1->setPosition(Vec2(0, -60));
	worldBackgroundBlack1->setFlippedY(true);
	node->addChild(worldBackgroundBlack);
	node->addChild(worldBackgroundBlack1);
}

void SelectWorldScene::createScrollView()
{
	auto size = Director::getInstance()->getVisibleSize();
	_scrollView = ui::ScrollView::create();
	_scrollView->setBounceEnabled(true);
	_scrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	_scrollView->setContentSize(size);
	_scrollView->setInnerContainerSize(Size(2700, size.height));
	_scrollView->setPosition(Vec2(0, 0));
	_scrollView->jumpToPercentHorizontal(_selectWorldPosition);
	this->addChild(_scrollView);
}

void SelectWorldScene::showDifferentWorlds()
{
	const string worldImageName[] = { {"World1"},{"World12"} ,{"World3"} ,{"World4"} ,{"World5"} ,{"World6"} ,{"World7"} ,{"World8"} ,{"World9"} ,{"World10"} ,{"World11"},{"World12"} };
	const string worldName[] = { {"现代世界"},{"尽请期待"} ,{"黑暗时代"} ,{"海盗港湾"} ,{"狂野西部"} ,{"冰河世纪"} ,{"未来世界"} ,{"侏罗纪世界"} ,{"大浪沙滩"} ,{"魔音时代"} ,{"失落之城"},{"尽情期待"} };
	for (int i = 0; i < 2; ++i)
	{
		_world[i] = ui::Button::create(worldImageName[i] + ".png", "", "", TextureResType::PLIST);
		_world[i]->setPosition(Vec2(1000 + 800 * i, _backgroundSize.height / 2.0f));
		_world[i]->setScale(1.7f);
		if (_global->userInformation->getMainToWorld()) _world[i]->setEnabled(false);
		_scrollView->addChild(_world[i]);

		auto worldname = Text::create();
		worldname->setPosition((Vec2)(_world[i]->getContentSize() / 2.0f) - Vec2(0, 100));
		worldname->setFontName(GAME_FONT_NAME_1);
		worldname->setFontSize(50);
		worldname->setColor(Color3B(0, 255, 255));
		worldname->setString(worldName[i]);
		_world[i]->addChild(worldname);

		if (i > 1)
		{
			_world[i]->setEnabled(false);
			worldname->setString("尽请期待");
		}

		_world[i]->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case ui::Widget::TouchEventType::BEGAN:
					PlayMusic::playMusic("tap");
					break;
				case ui::Widget::TouchEventType::ENDED:
					switch (i)
					{
					case 0:
						_global->userInformation->setIsMirrorScene(false);
						_global->userInformation->setSelectWorldName(WorldName::Mordern); /* 初始化背景 */
						Director::getInstance()->replaceScene(TransitionFade::create(1.f, World_1::createScene()));
						break;
					case 1:
						//_global->userInformation->setIsMirrorScene(true);
						//_global->userInformation->setSelectWorldName(WorldName::Mordern); /* 初始化背景 */
						//Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MirrorWorld_1::createScene()));
						break;
					default:
						break;
					}
					break;
				}
				UserData::getInstance()->caveUserData("SELECTWORLDPOSITION",
					_scrollView->getScrolledPercentHorizontal());
			});
	}
}
