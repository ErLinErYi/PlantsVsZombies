/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#include "GSButtonLayer.h"
#include "GSPauseQuitLayer.h"
#include "GSRequirementLayer.h"
#include "GSInformationLayer.h"
#include "GSDefine.h"

#include "Based/UserWinRequirement.h"
#include "Based/PlayMusic.h"
#include "Scenes/SelectPlantsScene/SPSSpriteLayer.h"

float PlantsInformation::PlantsCardFileData::PlantsSurPlusCoolTime[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };
float PlantsInformation::PlantsCardFileData::PlantsSurPlusPrecent[13] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 };

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
	auto button = Button::create(normal + ".png", select + ".png", "", TextureResType::PLIST);
	button->setPosition(position);
	button->setScale(0.7f);
	button->setFlippedX(isFlippedX);
	this->addChild(button);
	button->addTouchEventListener([&,buttonName](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("gravebutton");
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				switch (buttonName)
				{
				case GSLayerButton::stopButton:
					PlayMusic::playMusic("pause");
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
			PlayMusic::playMusic("pause");
			createRequirementLayer();
		}
		break;
		case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_W:                 /* 大小写w或者向上箭头加速 */
		case cocos2d::EventKeyboard::KeyCode::KEY_W:
		case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
			PlayMusic::playMusic("gravebutton");
			controlAccelerateScheduler();
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_CAPITAL_S:                /* 大小写s或者向下箭头减速 */
		case cocos2d::EventKeyboard::KeyCode::KEY_S:
		case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			PlayMusic::playMusic("gravebutton");
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
	_director->getRunningScene()->addChild(GSPauseQuitLayer::addLayer(), 10, "pauseLayer");
}

void GSButtonLayer::createRequirementButton()
{
	/* 如果有要求则显示通过要求 */
	if (_openLevelData->readLevelData(_openLevelData->getLevelNumber())->getGameType().size())
	{
		auto Requirement = ui::Button::create("Requirement.png", "RequirementDown.png", "", TextureResType::PLIST);
		Requirement->setPosition(Vec2(1570, 1030));
		Requirement->setScale(0.7f);
		this->addChild(Requirement);
		Requirement->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case ui::Widget::TouchEventType::ENDED:
					PlayMusic::playMusic("pause");
					createRequirementLayer();
					break;
				}
			});
	}
}

void GSButtonLayer::createRequirementLayer()
{
	GSPauseQuitLayer::pauseLayer();
	_director->getRunningScene()->addChild(GSRequirementLayer::addLayer(), 10);
}

void GSButtonLayer::controlAccelerateScheduler()
{
	if (Director::getInstance()->getScheduler()->getTimeScale() == 2.0f)
	{
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);
		_accelerateButton->loadTextureNormal("SpeedButton.png", TextureResType::PLIST);
	}
	else
	{
		Director::getInstance()->getScheduler()->setTimeScale(2.0f);
		_decelerateButton->loadTextureNormal("SpeedButton.png", TextureResType::PLIST);
		_accelerateButton->loadTextureNormal("SpeedButtonDown.png", TextureResType::PLIST);
	}
}

void GSButtonLayer::controlDecelerateScheduler()
{
	if (Director::getInstance()->getScheduler()->getTimeScale() == 0.5f)
	{
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);
		_decelerateButton->loadTextureNormal("SpeedButton.png", TextureResType::PLIST);
	}
	else
	{
		Director::getInstance()->getScheduler()->setTimeScale(0.5f);
		_accelerateButton->loadTextureNormal("SpeedButton.png", TextureResType::PLIST);
		_decelerateButton->loadTextureNormal("SpeedButtonDown.png", TextureResType::PLIST);
	}
}

void GSButtonLayer::showSeedBank()
{
	auto seedBank = Scale9Sprite::createWithSpriteFrameName("SeedBank.png");
	seedBank->setPosition(Vec2(0, 1080));
	seedBank->setAnchorPoint(Vec2(0, 1));
	seedBank->setContentSize(Size(210, 967));
	this->addChild(seedBank);
}

void GSButtonLayer::showShovelBank()
{
	auto shovelBank = Button::create("ShovelBank.png", "", "", TextureResType::PLIST);
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
				PlayMusic::playMusic("shovel");
				break;
			case ui::Widget::TouchEventType::ENDED:
				switch (mouseSelectImage->isSelectShovel)
				{
				case true:
					Director::getInstance()->getOpenGLView()->setCursor("resources/images/System/cursor.png", Point::ANCHOR_TOP_LEFT);
					mouseSelectImage->isSelectShovel = false;
					break;
				case false:
					if (_global->userInformation->getIsSelectCursorNotHide() == cocos2d::ui::CheckBox::EventType::UNSELECTED)
						Director::getInstance()->getOpenGLView()->setCursor("resources/images/System/Shovel_hi_res.png", Point::ANCHOR_BOTTOM_LEFT);
					else
						Director::getInstance()->getOpenGLView()->setCursor("resources/images/System/Shovel_hi_res1.png", Vec2(0.125, 0.2f));
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
		auto cardBackgroundImag = Button::create("SeedPacket_Larger.png", "SeedPacket_Larger.png", "", TextureResType::PLIST);
		cardBackgroundImag->setPosition(Vec2(105, 1008 - 103 * ++i));
		cardBackgroundImag->setEnabled(false);
		cardBackgroundImag->setColor(Color3B::GRAY);
		cardBackgroundImag->setCascadeColorEnabled(true);
		this->addChild(cardBackgroundImag);

		SPSSpriteLayer* sps_spriteLayer = new SPSSpriteLayer;
		auto cardInformation = sps_spriteLayer->showPlantsInformation(cardBackgroundImag, card.cardTag);
		delete sps_spriteLayer;

		ProgressTimer* timerBar;
		if (_global->userInformation->getIsReadFileData())
			timerBar = createProgressTimer(cardBackgroundImag,
				PlantsInformation::PlantsCardFileData::PlantsSurPlusCoolTime[card.cardTag],
				PlantsInformation::PlantsCardFileData::PlantsSurPlusPrecent[card.cardTag], card.cardTag);
		else
		{
			if (card.cardTag) timerBar = createProgressTimer(cardBackgroundImag, plantsInformation->PlantsCoolTime[card.cardTag], 100, card.cardTag);
			else timerBar = createProgressTimer(cardBackgroundImag, 0, 100, card.cardTag);
		}

		plantsCards[card.cardTag].plantsCards = cardBackgroundImag;
		plantsCards[card.cardTag].tag = card.cardTag;
		plantsCards[card.cardTag].plantsCardText = cardInformation;
		plantsCards[card.cardTag].plantsNeedSunNumbers = plantsInformation->PlantsNeedSunNumbers[card.cardTag];
		plantsCards[card.cardTag].progressTimer = timerBar;
	}
}

ProgressTimer* GSButtonLayer::createProgressTimer(Button* button, const float _time, const int from, const unsigned int& id)
{
	auto timerBar = ProgressTimer::create(Sprite::createWithSpriteFrameName("SeedPacketSilhouette1.png"));
	timerBar->setType(ProgressTimer::Type::BAR);  //设置条形进度条
	timerBar->setBarChangeRate(Vec2(1, 0));       //设置横向
	timerBar->setMidpoint(Vec2(0, 1));            //设置从左往右
	timerBar->setPosition(Vec2(90, 48));
	timerBar->setOpacity(150);
	timerBar->setPercentage(100);
	timerBar->runAction(Sequence::create(ProgressFromTo::create(_time, from, 0),
		CallFunc::create([=]() {
			plantsCards[id].timeBarIsFinished = true;
			}), nullptr));
	button->addChild(timerBar);
	return timerBar;
}