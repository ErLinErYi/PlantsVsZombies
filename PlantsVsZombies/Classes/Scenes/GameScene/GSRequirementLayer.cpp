/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.19
 *Email: 2117610943@qq.com
 */

#include "GSRequirementLayer.h"
#include "GSPauseQuitLayer.h"
#include "Based/GameType.h"

int pressKeySpace = 0;

Layer* GSRequirementLayer::addLayer()
{
	auto layer = GSRequirementLayer::create();
	return layer;
}

bool GSRequirementLayer::init()
{
	if (!Layer::init())return false;

	showRequirement();

	keyboardControl();

	return true;
}

void GSRequirementLayer::showRequirement()
{
	_requirement = new UserWinRequirement(this);
	_requirement->createDialogBox(GameTypes::None);
	_requirement->setShowDialogAction();

	auto button = Button::create(Global::getInstance()->userInformation->getImagePath().find("Continue1")->second, Global::getInstance()->userInformation->getImagePath().find("Continue")->second);
	button->setTitleText(Global::getInstance()->userInformation->getGameText().find("È·¶¨")->second);
	button->setTitleFontName("resources/fonts/fzse_gbk.ttf");
	button->setTitleFontSize(30);
	button->setTitleColor(Color3B::YELLOW);
	button->setPosition(Vec2(_requirement->getDialog()->getContentSize().width / 2.0f, 20));
	button->setGlobalZOrder(10);
	button->runAction(FadeIn::create(0.5f));
	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(Global::getInstance()->userInformation->getMusicPath().find("gravebutton")->second), Global::getInstance()->userInformation->getSoundEffectVolume());
				break;
			case ui::Widget::TouchEventType::ENDED:
				_requirement->setDelectDialogAction();
				this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([&]()
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
			AudioEngine::setVolume(AudioEngine::play2d(Global::getInstance()->userInformation->getMusicPath().find("tap")->second), Global::getInstance()->userInformation->getSoundEffectVolume());
			_requirement->setDelectDialogAction();
			this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([&, KeyBoard]()
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
