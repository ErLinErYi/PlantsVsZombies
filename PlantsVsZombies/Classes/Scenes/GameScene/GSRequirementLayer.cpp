/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.19
 *Email: 2117610943@qq.com
 */

#include "GSRequirementLayer.h"
#include "GSPauseQuitLayer.h"
#include "GameScene.h"

#include "Based/GameType.h"
#include "Based/PlayMusic.h"

int pressKeySpace = 0;

Layer* GSRequirementLayer::addLayer()
{
	return GSRequirementLayer::create();
}

bool GSRequirementLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	showRequirement();
	keyboardControl();

	return true;
}

void GSRequirementLayer::showRequirement()
{
	_requirement = new UserWinRequirement(this);
	_requirement->createDialogBox(GameTypes::None);
	_requirement->setShowDialogAction();

	auto button = Button::create("Continue1.png", "Continue.png", "", TextureResType::PLIST);
	button->setTitleText(Global::getInstance()->userInformation->getGameText().find("È·¶¨")->second);
	button->setTitleFontName(GAME_FONT_NAME_1);
	button->setTitleFontSize(30);
	button->setTitleColor(Color3B::YELLOW);
	button->setPosition(Vec2(_requirement->getDialog()->getContentSize().width / 2.0f, 10));
	button->setGlobalZOrder(10);
	button->setScale(0.5f);
	button->runAction(FadeIn::create(0.5f));
	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("gravebutton");
				break;
			case ui::Widget::TouchEventType::ENDED:
				_requirement->setDelectDialogAction();
				this->runAction(Sequence::create(Spawn::create(DelayTime::create(0.2f), FadeOut::create(0.2f), nullptr),
					CallFunc::create([&]()
					{
						this->removeFromParent();
						GSPauseQuitLayer::resumeLayer();
					}), nullptr));
				break;
			}
		});
	_requirement->getDialog()->addChild(button);
}

void GSRequirementLayer::keyboardControl()
{
	auto KeyBoard = EventListenerKeyboard::create();
	KeyBoard->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* event)
	{
		switch (code)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:                     /* ¿Õ¸ñ¼ü»Ö¸´ */
			KeyBoard->setEnabled(false);
			PlayMusic::playMusic("tap");
			_requirement->setDelectDialogAction();
			this->runAction(Sequence::create(Spawn::create(DelayTime::create(0.2f), FadeOut::create(0.2f), nullptr),
				CallFunc::create([&, KeyBoard]()
				{
					this->removeFromParent();
					GSPauseQuitLayer::resumeLayer();
					KeyBoard->setEnabled(true);
				}), nullptr));
			break;
		default:
			break;
		}
	};
	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(KeyBoard, this);
}
