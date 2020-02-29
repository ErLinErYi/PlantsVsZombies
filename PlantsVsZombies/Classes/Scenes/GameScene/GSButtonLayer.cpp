/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#include "GSButtonLayer.h"
#include "GSPauseQuitLayer.h"
#include "Based/UserWinRequirement.h"
#include "Scenes/SelectPlantsScene/SPSSpriteLayer.h"
#include "GSRequirementLayer.h"
#include "GSInformationLayer.h"

#include "GSDefine.h"

GSButtonLayer::GSButtonLayer():
	_global(Global::getInstance()),
	_director(Director::getInstance()),
	_openLevelData(OpenLevelData::getInstance()),
	_quitLayer(nullptr),
	_accelerateButton(nullptr),
	_decelerateButton(nullptr),
	mouseSelectImage(new MouseSelectImage),
	plantsInformation(new PlantsInformation)
{
}

GSButtonLayer::~GSButtonLayer()
{
	delete mouseSelectImage;
	delete plantsInformation;
}

bool GSButtonLayer::init()
{
	if(!Layer::init())return false;

	showSeedBank();
	showShovelBank();
	createPlantsCard();
	createRequirementButton();
	createButton("StopButton", "StopButtonDown", Vec2(1870, 1030), GSLayerButton::stopButton);
	_accelerateButton = createButton("SpeedButton", "SpeedButtonDown", Vec2(1770, 1030), GSLayerButton::accelerateButton);
	_decelerateButton = createButton("SpeedButton", "SpeedButtonDown", Vec2(1670, 1030), GSLayerButton::decelerateButton, true);
	createKeyBoardListener();

	return true;
}

Button* GSButtonLayer::createButton(const string& normal, const string& select, const Vec2& position, GSLayerButton buttonName, const bool isFlippedX)
{
	auto button = Button::create(_global->userInformation->getImagePath().find(normal)->second, _global->userInformation->getImagePath().find(select)->second);
	button->setPosition(position);
	button->setScale(0.7f);
	button->setFlippedX(isFlippedX);
	this->addChild(button);
	button->addTouchEventListener([&,buttonName](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("gravebutton")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				switch (buttonName)
				{
				case GSLayerButton::stopButton:
					AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("pause")->second), _global->userInformation->getSoundEffectVolume());
					createQuitDialog();
					break;
				case GSLayerButton::accelerateButton:
					controlAccelerateScheduler();
					break;
				case GSLayerButton::decelerateButton:
					controlDecelerateScheduler();
					break;
				}
				break;
			}
		});
	return button;
}

void GSButtonLayer::createKeyBoardListener()
{
	/* 创建键盘监听 */
	auto KeyBoard = EventListenerKeyboard::create();
	KeyBoard->onKeyReleased = [&](EventKeyboard::KeyCode code, Event* event)
	{
		switch (code)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:                     /* 空格键暂停 */
			//if (KEY_PRESS(pressKeySpace))
			{
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("gravebutton")->second), _global->userInformation->getSoundEffectVolume());
				createRequirementLayer();
			}
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:                 /* 大小写w或者向上箭头加速 */
		case cocos2d::EventKeyboard::KeyCode::KEY_W:
		case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("gravebutton")->second), _global->userInformation->getSoundEffectVolume());
			controlAccelerateScheduler();
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:                /* 大小写s或者向下箭头减速 */
		case cocos2d::EventKeyboard::KeyCode::KEY_S:
		case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("gravebutton")->second), _global->userInformation->getSoundEffectVolume());
			controlDecelerateScheduler();
			break;
		default:
			break;
		}
	};
	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(KeyBoard, this);
}

void GSButtonLayer::createQuitDialog()
{
	GSPauseQuitLayer::pauseLayer();

	_director->getRunningScene()->addChild(GSPauseQuitLayer::addLayer(), 10);
}

void GSButtonLayer::createRequirementButton()
{
	/* 如果有要求则显示通过要求 */
	if (_openLevelData->readLevelData(_openLevelData->getLevelNumber())->getGameType().size())
	{
		auto Requirement = ui::Button::create(_global->userInformation->getImagePath().find("Requirement")->second, _global->userInformation->getImagePath().find("RequirementDown")->second);
		Requirement->setPosition(Vec2(1570, 1030));
		Requirement->setScale(0.7f);
		this->addChild(Requirement);
		Requirement->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case ui::Widget::TouchEventType::BEGAN:
					AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("gravebutton")->second), _global->userInformation->getSoundEffectVolume());
					break;
				case ui::Widget::TouchEventType::ENDED:
					createRequirementLayer();
					break;
				}
			});
	}
}

void GSButtonLayer::createRequirementLayer()
{
	GSPauseQuitLayer::pauseLayer();
	_director->getRunningScene()->addChild(GSRequirementLayer::addLayer());
}

void GSButtonLayer::controlAccelerateScheduler()
{
	if (Director::getInstance()->getScheduler()->getTimeScale() == 1.5f)
	{
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);
		_accelerateButton->loadTextureNormal(_global->userInformation->getImagePath().find("SpeedButton")->second);
	}
	else
	{
		Director::getInstance()->getScheduler()->setTimeScale(1.5f);
		_decelerateButton->loadTextureNormal(_global->userInformation->getImagePath().find("SpeedButton")->second);
		_accelerateButton->loadTextureNormal(_global->userInformation->getImagePath().find("SpeedButtonDown")->second);
	}
}

void GSButtonLayer::controlDecelerateScheduler()
{
	if (Director::getInstance()->getScheduler()->getTimeScale() == 0.5f)
	{
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);
		_decelerateButton->loadTextureNormal(_global->userInformation->getImagePath().find("SpeedButton")->second);
	}
	else
	{
		Director::getInstance()->getScheduler()->setTimeScale(0.5f);
		_accelerateButton->loadTextureNormal(_global->userInformation->getImagePath().find("SpeedButton")->second);
		_decelerateButton->loadTextureNormal(_global->userInformation->getImagePath().find("SpeedButtonDown")->second);
	}
}

void GSButtonLayer::showSeedBank()
{
	auto seedBank = Scale9Sprite::create(_global->userInformation->getImagePath().find("SeedBank")->second);
	seedBank->setPosition(Vec2(0, 1080));
	seedBank->setAnchorPoint(Vec2(0, 1));
	seedBank->setCapInsets(Rect(0, 5, 87, 373));
	seedBank->setContentSize(Size(217.5f, 980));
	this->addChild(seedBank);
}

void GSButtonLayer::showShovelBank()
{
	auto shovelBank = Button::create(_global->userInformation->getImagePath().find("ShovelBank")->second);
	shovelBank->setPosition(_openLevelData->readLevelData(_openLevelData->getLevelNumber())->getGameType().size() > 0 ? Vec2(1420, 1080) : Vec2(1520, 1080));
	shovelBank->setScale(0.6f);
	shovelBank->setAnchorPoint(Vec2(0, 1));
	shovelBank->setName("ShovelBank");
	this->addChild(shovelBank);

	/* 铲子监听 */
	shovelBank->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("shovel")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case ui::Widget::TouchEventType::ENDED:
				switch (mouseSelectImage->isSelectShovel)
				{
				case true:
					Director::getInstance()->getOpenGLView()->setCursor(_global->userInformation->getImagePath().find("cursor")->second, Point::ANCHOR_TOP_LEFT);
					mouseSelectImage->isSelectShovel = false;
					break;
				case false:
					if (_global->userInformation->getIsSelectCursorNotHide() == cocos2d::ui::CheckBox::EventType::UNSELECTED)
					{
						Director::getInstance()->getOpenGLView()->setCursor(_global->userInformation->getImagePath().find("Shovel_hi_res")->second, Point::ANCHOR_BOTTOM_LEFT);
					}
					else
					{
						Director::getInstance()->getOpenGLView()->setCursor(_global->userInformation->getImagePath().find("Shovel_hi_res_1")->second, Vec2(0.125, 0.2f));
					}
					mouseSelectImage->isSelectShovel = true;
					break;
				}
				break;
			}
		});
}

void GSButtonLayer::createPlantsCard()
{
	int i = -1;
	for (auto& card : _global->userInformation->getUserSelectCrads())
	{
		auto cardBackgroundImag = Button::create(_global->userInformation->getImagePath().find("SeedPacket_Larger")->second, _global->userInformation->getImagePath().find("SeedPacket_Larger")->second);
		cardBackgroundImag->setPosition(Vec2(110, 1008 - 103 * ++i));
		cardBackgroundImag->setEnabled(false);
		cardBackgroundImag->setColor(Color3B::GRAY);
		cardBackgroundImag->setCascadeColorEnabled(true);
		this->addChild(cardBackgroundImag);

		SPSSpriteLayer* sps_spriteLayer = new SPSSpriteLayer;
		auto cardInformation = sps_spriteLayer->showPlantsInformation(cardBackgroundImag, card.cardTag);
		delete sps_spriteLayer;

		ProgressTimer* timerBar;
		if (card.cardTag) timerBar = createProgressTimer(cardBackgroundImag, plantsInformation->PlantsCoolTime[card.cardTag], card.cardTag);
		else timerBar = createProgressTimer(cardBackgroundImag, 0, card.cardTag);

		plantsCards[card.cardTag].plantsCards = cardBackgroundImag;
		plantsCards[card.cardTag].tag = card.cardTag;
		plantsCards[card.cardTag].plantsCardText = cardInformation;
		plantsCards[card.cardTag].plantsNeedSunNumbers = plantsInformation->PlantsNeedSunNumbers[card.cardTag];
		plantsCards[card.cardTag].progressTimer = timerBar;
	}
}

ProgressTimer* GSButtonLayer::createProgressTimer(Button* button, const float _time, const int& id)
{
	auto timerBar = ProgressTimer::create(Sprite::create(_global->userInformation->getImagePath().find("SeedPacketSilhouette1")->second));
	timerBar->setType(ProgressTimer::Type::BAR);  //设置条形进度条
	timerBar->setBarChangeRate(Vec2(1, 0));       //设置横向
	timerBar->setMidpoint(Vec2(0, 1));            //设置从左往右
	timerBar->setPosition(Vec2(90, 48));
	timerBar->setOpacity(150);
	timerBar->setPercentage(100);
	timerBar->runAction(Sequence::create(ProgressFromTo::create(_time, 100, 0), 
		CallFunc::create([=]() {
			plantsCards[id].timeBarIsFinished = true;
			}), nullptr));
	button->addChild(timerBar);
	return timerBar;
}