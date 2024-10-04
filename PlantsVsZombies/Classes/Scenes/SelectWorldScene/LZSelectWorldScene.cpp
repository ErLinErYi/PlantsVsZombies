/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.15
 *Modify: 2020.1.28
 *Email: 2117610943@qq.com
 */

#include "LZSelectWorldScene.h"
#include "LZSWSUnlockDialogLayer.h"
#include "Scenes/GameScenes/Adventure/WorldScene/LZModernWorld.h"
#include "Scenes/GameScenes/BigMap/WorldScene/LZBigMapWorld.h"
#include "Scenes/MainMenuScene/LZMainMenu.h"

#include "Based/LZUserData.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZMouseEventControl.h"
#include "AudioEngine.h"


SelectWorldScene::SelectWorldScene():
	_scrollView(nullptr), 
	_global(Global::getInstance())
{
	_selectWorldPosition = UserData::getInstance()->openDoubleUserData(const_cast<char*>("SELECTWORLDPOSITION"));
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
		const auto mouseEvent = static_cast<EventMouse*>(event);
		const float movex = mouseEvent->getScrollY() * 40;

		MouseEventControl::mouseScrollControlListener(_scrollView, movex, ScrollView::Direction::HORIZONTAL);
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
	auto back = cocos2d::ui::Button::create("back.png", "back1.png", "",cocos2d::ui::Widget::TextureResType::PLIST);
	back->setScale(0.7f);
	back->setAnchorPoint(Vec2(0, 1));
	back->setPosition(Vec2(0, 1080));
	this->addChild(back);

	back->addTouchEventListener([=](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("gravebutton");
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				Director::getInstance()->replaceScene(MainMenu::create());
				UserData::getInstance()->caveUserData(const_cast<char*>("SELECTWORLDPOSITION"),
					_scrollView->getScrolledPercentHorizontal());
				break;
			}
		});
}

void SelectWorldScene::createSelectDifficulty()
{
	_global->userInformation->setGameDifficulty(UserData::getInstance()->openIntUserData(const_cast<char*>("DIFFICULTY")));
		
	auto radioButtonGroup = RadioButtonGroup::create();
	radioButtonGroup->setAllowedNoSelection(false);
	this->addChild(radioButtonGroup);

	auto radioButton = RadioButton::create();
	radioButton->loadTextureBackGround("CheckBox2.png", cocos2d::ui::Widget::TextureResType::PLIST);
	radioButton->loadTextureFrontCross("CheckBox.png", cocos2d::ui::Widget::TextureResType::PLIST);
	radioButton->setPosition(Vec2(103, 1080));
	radioButton->setAnchorPoint(Vec2(0, 1));
	radioButtonGroup->addRadioButton(radioButton);
	this->addChild(radioButton);

	auto radioButton2 = RadioButton::create();
	radioButton2->loadTextureBackGround("CheckBox2.png", cocos2d::ui::Widget::TextureResType::PLIST);
	radioButton2->loadTextureFrontCross("CheckBox.png", cocos2d::ui::Widget::TextureResType::PLIST);
	radioButton2->setPosition(Vec2(203, 1080));
	radioButton2->setAnchorPoint(Vec2(0, 1));
	radioButtonGroup->addRadioButton(radioButton2);
	this->addChild(radioButton2);

	auto radioButton3 = RadioButton::create();
	radioButton3->loadTextureBackGround("CheckBox2.png", cocos2d::ui::Widget::TextureResType::PLIST);
	radioButton3->loadTextureFrontCross("CheckBox.png", cocos2d::ui::Widget::TextureResType::PLIST);
	radioButton3->setPosition(Vec2(303, 1080));
	radioButton3->setAnchorPoint(Vec2(0, 1));
	radioButtonGroup->addRadioButton(radioButton3);
	this->addChild(radioButton3);

	auto radioButton4 = RadioButton::create();
	radioButton4->loadTextureBackGround("CheckBox2.png", cocos2d::ui::Widget::TextureResType::PLIST);
	radioButton4->loadTextureFrontCross("CheckBox.png", cocos2d::ui::Widget::TextureResType::PLIST);
	radioButton4->setPosition(Vec2(403, 1080));
	radioButton4->setAnchorPoint(Vec2(0, 1));
	radioButtonGroup->addRadioButton(radioButton4);
	this->addChild(radioButton4);

	radioButtonGroup->setSelectedButton(_global->userInformation->getGameDifficulty());

	if (checkModeUnlock() < 3)radioButton4->setEnabled(false), radioButton4->setOpacity(0);
	if (checkModeUnlock() < 2)radioButton3->setEnabled(false), radioButton3->setOpacity(100);
	if (checkModeUnlock() < 1)radioButton2->setEnabled(false), radioButton2->setOpacity(100);
	
	modeButtonText(radioButton, "简单模式", Vec2(47, -20), Color4B(255, 255, 255, 200), Color4B(0, 0, 255, 255));
	modeButtonText(radioButton2, "噩梦模式", Vec2(47, -20), Color4B(0, 255, 255, 200), Color4B(255, 255, 255, 255));
	modeButtonText(radioButton3, "地狱模式", Vec2(47, -20), Color4B(255, 0, 255, 200), Color4B(255, 255, 255, 255));
	modeButtonText(radioButton4, "炼狱模式", Vec2(47, -20), Color4B(255, 0, 0, 200), Color4B(255, 255, 255, 255));

	modeButtonCallBack(radioButton, 0);
	modeButtonCallBack(radioButton2, 1);
	modeButtonCallBack(radioButton3, 2);
	modeButtonCallBack(radioButton4, 3);
}

void SelectWorldScene::modeButtonCallBack(RadioButton* button, int worldId)
{
	button->addEventListener([=](RadioButton* sender, RadioButton::EventType type)
		{
			PlayMusic::playMusic("bleep");
			switch (type)
			{
			case cocos2d::ui::RadioButton::EventType::SELECTED:
				_global->userInformation->setGameDifficulty(worldId);
				break;
			case cocos2d::ui::RadioButton::EventType::UNSELECTED:
				break;
			default:
				break;
			}
			for (int i = 0; i < WorldNumbers; ++i)
			{
				_global->userInformation->getUserSelectWorldData().at(i)->isReadWoldInformation = false;
			}
			UserData::getInstance()->caveUserData(const_cast<char*>("DIFFICULTY"), _global->userInformation->getGameDifficulty());
		});
}

void SelectWorldScene::modeButtonText(RadioButton* button, const string& modeName, const Vec2& position, const Color4B& color1, const Color4B& color2)
{
	auto text = Text::create();
	text->setPosition(position);
	text->setTextColor(color1);
	text->setFontName(GAME_FONT_NAME_1);
	text->setFontSize(_global->userInformation->getGameText().find(modeName)->second->fontsize - 5);
	text->setString(_global->userInformation->getGameText().find(modeName)->second->text);
	text->enableGlow(color2);
	button->addChild(text);
	text->setOpacity(text->getParent()->getOpacity());
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
				for (int i = 0; i < 3; ++i)
				{
					_world[i]->setEnabled(true);
				}
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
	const auto size = Director::getInstance()->getVisibleSize();
	_scrollView = cocos2d::ui::ScrollView::create();
	_scrollView->setBounceEnabled(true);
	_scrollView->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
	_scrollView->setContentSize(size);
	_scrollView->setInnerContainerSize(Size(3500, size.height));
	_scrollView->setPosition(Vec2(0, 0));
	_scrollView->jumpToPercentHorizontal(_selectWorldPosition);
	this->addChild(_scrollView);
}

void SelectWorldScene::showDifferentWorlds()
{
	const string worldImageName[] = { {"World1"},{"World2"} ,{"World12"} ,{"World12"} ,{"World5"} ,{"World6"} ,{"World7"} ,{"World8"} ,{"World9"} ,{"World10"} ,{"World11"},{"World12"} };
	const string worldName[] = { {"现代世界"},{"大地图世界"} ,{"尽请期待"} ,{"尽请期待"} ,{"狂野西部"} ,{"冰河世纪"} ,{"未来世界"} ,{"侏罗纪世界"} ,{"大浪沙滩"} ,{"魔音时代"} ,{"失落之城"},{"尽请期待"} };
	for (int i = 0; i < WorldNumbers + 1; ++i)
	{
		_world[i] = cocos2d::ui::Button::create(worldImageName[i] + ".png", "", "",cocos2d::ui::Widget::TextureResType::PLIST);
		_world[i]->setPosition(Vec2(1000 + 800 * i, _backgroundSize.height / 2.0f));
		_world[i]->setScale(i == 1 ? 1.f : 1.7f);
		if (_global->userInformation->getMainToWorld()) _world[i]->setEnabled(false);
		_scrollView->addChild(_world[i]);

		auto worldname = Text::create();
		worldname->setPosition(Vec2(1000 + 800 * i, _backgroundSize.height / 2.0f));
		worldname->setFontName(GAME_FONT_NAME_1);
		worldname->setFontSize(_global->userInformation->getGameText().find(worldName[i])->second->fontsize);
		worldname->setColor(Color3B(0, 255, 255));
		worldname->setString(_global->userInformation->getGameText().find(worldName[i])->second->text);
		worldname->enableGlow(Color4B(255, 0, 255, 255));
		worldname->setScale(1.2f);
		_scrollView->addChild(worldname);

		if (i == 1 && !checkWorldUnlock())
		{
			_world[i]->setColor(Color3B::BLACK);
		}
		if (i >= 2)
		{
			// _world[i]->setEnabled(false);
			worldname->setString(_global->userInformation->getGameText().find("尽请期待")->second->text);
		}

		_world[i]->addTouchEventListener([=](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case cocos2d::ui::Widget::TouchEventType::ENDED:
					PlayMusic::playMusic("tap");
					switch (i)
					{
					case 0:
						_global->userInformation->setIsMirrorScene(false);
						_global->userInformation->setSelectWorldName(WorldName::Mordern); /* 初始化背景 */
						Director::getInstance()->replaceScene(TransitionFade::create(1.f, ModernWorld::create()));
						break;
					case 1:
						if (checkWorldUnlock())
						{
							_global->userInformation->setIsMirrorScene(false);
							_global->userInformation->setSelectWorldName(WorldName::BigMap); /* 初始化背景 */
							Director::getInstance()->replaceScene(TransitionFade::create(1.0f, BigMapWorld::create()));
						}
						else
						{
							this->addChild(SWSUnlockDialogLayer::create());
						}
						break;
					case 2:
						break;
					default:
						break;
					}
					break;
				}
				UserData::getInstance()->caveUserData(const_cast<char*>("SELECTWORLDPOSITION"), _scrollView->getScrolledPercentHorizontal());
			});
	}
}

int SelectWorldScene::checkWorldUnlock()
{
	auto gInfo = _global->userInformation;
	for (int i = 1; i <= WorldNumbers; ++i)
	{
		auto number = UserData::getInstance()->openIntUserData(const_cast<char*>(
			StringUtils::format(gInfo->getSystemCaveFileName().c_str(), i, 0).c_str()));
		if (number <= static_cast<int>(SWSUnlockDialogLayer::unlockNeedNumbers))
		{
			return i - 1;
		}
	}
	return WorldNumbers;
}

int SelectWorldScene::checkModeUnlock(int id)
{
	auto gInfo = _global->userInformation;
	for (int i = 0; i < 4; ++i)
	{
		auto number = UserData::getInstance()->openIntUserData(const_cast<char*>(
			StringUtils::format(gInfo->getSystemCaveFileName().c_str(), id, i).c_str()));
		if (number <= static_cast<int>(SWSUnlockDialogLayer::unlockNeedNumbers))
		{
			return i;
		}
	}
	return 4;
}
