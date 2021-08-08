/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#include "LZAGSButtonLayer.h"
#include "LZAGSPauseQuitLayer.h"
#include "LZAGSRequirementLayer.h"
#include "LZAGSInformationLayer.h"
#include "LZAGSControlLayer.h"
#include "LZAGSZombiesAppearControl.h"
#include "LZAGSDefine.h"
#include "LZAGSData.h"

#include "Based/LZUserWinRequirement.h"
#include "Based/LZPlayMusic.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSSpriteLayer.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSJumpLevelLayer.h"

GSButtonLayer::GSButtonLayer():
	_global(Global::getInstance()),
	_director(Director::getInstance()),
	_openLevelData(OpenLevelData::getInstance()),
	_quitLayer(nullptr),
	_accelerateButton(nullptr),
	_decelerateButton(nullptr),
	_shovelBank(nullptr),
	nextWaveButton(nullptr),
	shovelImage(nullptr),
	mouseSelectImage(new MouseSelectImage)
{
}

GSButtonLayer::~GSButtonLayer()
{
	delete mouseSelectImage;
}

bool GSButtonLayer::init()
{
	if(!Layer::init())return false;

	showSeedBank();
	showShovelBank();
	showNextWaveButton();
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
	auto button = Button::create(normal + ".png", select + ".png", "",cocos2d::ui::Widget::TextureResType::PLIST);
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
					createQuitDialog();
					break;
				case GSLayerButton::accelerateButton:
					controlAccelerateScheduler();
					break;
				case GSLayerButton::decelerateButton:
					controlDecelerateScheduler();
					break;
				case GSLayerButton::recoverySunButton:
					recoverySunControl();
					break;
				case GSLayerButton::recoveryCoinButton:
					recoveryCoinControl();
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
	PlayMusic::playMusic("pause");
	_director->getRunningScene()->addChild(GSPauseQuitLayer::create(), 10, "pauseLayer");
}

void GSButtonLayer::createRequirementButton()
{
	/* 如果有要求则显示通过要求 */
	if (_openLevelData->readLevelData(_openLevelData->getLevelNumber())->getGameType().size())
	{
		auto Requirement = ui::Button::create("Requirement.png", "RequirementDown.png", "",cocos2d::ui::Widget::TextureResType::PLIST);
		Requirement->setPosition(Vec2(1570, 1030));
		Requirement->setScale(0.7f);
		this->addChild(Requirement);
		Requirement->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case ui::Widget::TouchEventType::ENDED:
					createRequirementLayer();
					break;
				}
			});
	}

	if (SPSControlLayer::isShowJumpLevelButton())
	{
		createJumpLevelButton();
	}
}

void GSButtonLayer::createRequirementLayer()
{
	GSPauseQuitLayer::pauseLayer();
	PlayMusic::playMusic("pause");
	_director->getRunningScene()->addChild(GSRequirementLayer::addLayer(), 10);
}

void GSButtonLayer::controlAccelerateScheduler()
{
	if (Director::getInstance()->getScheduler()->getTimeScale() == 2.0f)
	{
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);
		if (_accelerateButton)_accelerateButton->loadTextureNormal("SpeedButton.png",cocos2d::ui::Widget::TextureResType::PLIST);
	}
	else
	{
		Director::getInstance()->getScheduler()->setTimeScale(2.0f);
		if(_decelerateButton)_decelerateButton->loadTextureNormal("SpeedButton.png",cocos2d::ui::Widget::TextureResType::PLIST);
		if(_accelerateButton)_accelerateButton->loadTextureNormal("SpeedButtonDown.png",cocos2d::ui::Widget::TextureResType::PLIST);
	}
}

void GSButtonLayer::controlDecelerateScheduler()
{
	if (Director::getInstance()->getScheduler()->getTimeScale() == 0.7f)
	{
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);
		if (_decelerateButton)_decelerateButton->loadTextureNormal("SpeedButton.png",cocos2d::ui::Widget::TextureResType::PLIST);
	}
	else
	{
		Director::getInstance()->getScheduler()->setTimeScale(0.7f);
		if (_accelerateButton)_accelerateButton->loadTextureNormal("SpeedButton.png",cocos2d::ui::Widget::TextureResType::PLIST);
		if (_decelerateButton)_decelerateButton->loadTextureNormal("SpeedButtonDown.png",cocos2d::ui::Widget::TextureResType::PLIST);
	}
}

void GSButtonLayer::showSeedBank()
{
	auto seedBank = cocos2d::ui::Scale9Sprite::createWithSpriteFrameName("SeedBank.png");
	seedBank->setPosition(Vec2(0, 1080));
	seedBank->setAnchorPoint(Vec2(0, 1));
	seedBank->setContentSize(Size(210, 967));
	this->addChild(seedBank);
}

void GSButtonLayer::showShovelBank()
{
	_shovelBank = Button::create("ShovelBank.png", "", "",cocos2d::ui::Widget::TextureResType::PLIST);
	_shovelBank->setPosition(_openLevelData->readLevelData(_openLevelData->getLevelNumber())->getGameType().size() > 0 ? Vec2(1425, 1075) : Vec2(1525, 1075));
	_shovelBank->setScale(0.6f);
	_shovelBank->setAnchorPoint(Vec2(0, 1));
	_shovelBank->setName("ShovelBank");
	this->addChild(_shovelBank, 1);

	_shovelBank->addTouchEventListener([=](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("shovel");
				switch (mouseSelectImage->isSelectShovel)
				{
				case true:  deleteShovelImage(); break;
				case false: createShovelImage(); break;
				}
				break;
			}
		});
}

void GSButtonLayer::updateShovel(bool isHaveShovel)
{
	if (_shovelBank)
	{
		if (isHaveShovel)
		{
			_shovelBank->loadTextureNormal("ShovelBank.png", cocos2d::ui::Widget::TextureResType::PLIST);
		}
		else
		{
			_shovelBank->loadTextureNormal("ShovelBank1.png", cocos2d::ui::Widget::TextureResType::PLIST);
		}
	}
}

void GSButtonLayer::showNextWaveButton()
{
	nextWaveButton = Button::create("NextWave.png", "NextWave2.png", "",cocos2d::ui::Widget::TextureResType::PLIST);
	nextWaveButton->setPosition(getChildByName("ShovelBank")->getPosition() - Vec2(92, -5));
	nextWaveButton->setScale(0.6f);
	nextWaveButton->setAnchorPoint(Vec2(0, 1));
	nextWaveButton->setVisible(false);
	this->addChild(nextWaveButton);

	nextWaveButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("gravebutton");
				break;
			case ui::Widget::TouchEventType::ENDED:
				nextWaveButtonControl();
				break;
			}
		});
}

void GSButtonLayer::nextWaveButtonControl()
{
	nextWaveButton->setVisible(false);
	controlLayerInformation->_zombiesAppearControl->setTimeAdd(50);
}

void GSButtonLayer::createPlantsCard()
{
	int t = -1, n, m;
	for (int i = _global->userInformation->getUserSelectCrads().size() - 1; i >= 0; --i)
	{
		if (_global->userInformation->getUserSelectCrads()[i].cardTag == static_cast<int>(PlantsType::Imitater))
		{
			t = i - 1;
			break;
		}
	}
	if (t != -1)
	{
		n = _global->userInformation->getUserSelectCrads()[t].cardTag;
		m = _global->userInformation->getUserSelectCrads()[t + 1].cardTag;
	}

	int i = -1;
	for (auto& card : _global->userInformation->getUserSelectCrads())
	{
		auto imageBg = "SeedPacket" + to_string(plantsCardInformation[card.cardTag].quality) + ".png";
		auto cardBackgroundImag = Button::create(imageBg, imageBg, "",cocos2d::ui::Widget::TextureResType::PLIST);
		cardBackgroundImag->setPosition(Vec2(105, 1008 - 103 * ++i));
		cardBackgroundImag->setEnabled(false);
		cardBackgroundImag->setColor(Color3B::GRAY);
		cardBackgroundImag->setCascadeColorEnabled(true);
		cardBackgroundImag->setTag(card.cardTag);
		this->addChild(cardBackgroundImag);

		SPSSpriteLayer* sps_spriteLayer = new SPSSpriteLayer;
		Text* cardInformation = nullptr;
		if (plantsCardInformation[card.cardTag].type == PlantsType::Imitater)
		{
			cardBackgroundImag->setTag(_global->userInformation->getUserSelectCrads()[t].cardTag);
			cardInformation = sps_spriteLayer->showPlantsInformation(cardBackgroundImag);
			cardBackgroundImag->getChildByName("plantImage")->setGLProgram(greyScaleShader());
			cardBackgroundImag->setTag(card.cardTag);
		}
		else
		{
			cardInformation = sps_spriteLayer->showPlantsInformation(cardBackgroundImag);
		}
		delete sps_spriteLayer;

		ProgressTimer* timerBar;
		if (_global->userInformation->getIsReadFileData())
		{
			timerBar = createProgressTimer(cardBackgroundImag,
				plantsCardInformation[card.cardTag].PlantsSurPlusCoolTime,
				plantsCardInformation[card.cardTag].PlantsSurPlusPrecent, card.cardTag);
		}
		else
		{
			if (plantsCardInformation[card.cardTag].type != PlantsType::SunFlower &&
				plantsCardInformation[card.cardTag].type != PlantsType::SunFlowerTwin)
			{
				if (plantsCardInformation[card.cardTag].type == PlantsType::Imitater)
				{
					timerBar = createProgressTimer(cardBackgroundImag, plantsCardInformation[n].plantsCoolTime, 100, card.cardTag);
				}
				else
				{
					timerBar = createProgressTimer(cardBackgroundImag, plantsCardInformation[card.cardTag].plantsCoolTime, 100, card.cardTag);
				}
			}
			else
			{
				timerBar = createProgressTimer(cardBackgroundImag, 0, 100, card.cardTag);
			}
		}

		SPSSpriteLayer::createButtonHoverEffect(cardBackgroundImag);

		plantsCards[card.cardTag].plantsCards = cardBackgroundImag;
		plantsCards[card.cardTag].tag = card.cardTag;
		plantsCards[card.cardTag].plantsCardText = cardInformation;
		plantsCards[card.cardTag].plantsNeedSunNumbers = plantsCardInformation[card.cardTag].plantsNeedSunNumbers;
		plantsCards[card.cardTag].plantsCoolTime = plantsCardInformation[card.cardTag].plantsCoolTime;
		plantsCards[card.cardTag].progressTimer = timerBar;
	}

	if (t != -1)
	{
		// 设置模仿者数据
		plantsCards[m].plantsNeedSunNumbers = plantsCardInformation[n].plantsNeedSunNumbers;
		plantsCards[m].plantsCoolTime = plantsCardInformation[n].plantsCoolTime;
	}
}

void GSButtonLayer::createJumpLevelButton()
{
	auto breakButton = ui::Button::create("SeedChooser_Button.png", "SeedChooser_Button_Disabled.png", "",cocos2d::ui::Widget::TextureResType::PLIST);
	breakButton->setPosition(Vec2(1250, 5));
	breakButton->setScale(1.5f);
	breakButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	breakButton->setTitleText(_global->userInformation->getGameText().find("跳过此关")->second->text);
	breakButton->setTitleColor(Color3B(0, 255, 255));
	breakButton->setTitleFontName(GAME_FONT_NAME_1);
	breakButton->setTitleFontSize(_global->userInformation->getGameText().find("跳过此关")->second->fontsize);
	this->addChild(breakButton);
	breakButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("gravebutton");
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				createRequirementLayer();
				auto jump = SPSJumpLevelLayer::create();
				if (jump) {
					jump->setGlobalZOrder(10);
					_director->getRunningScene()->addChild(jump, 11);
				}
				break;
			}
		});
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

void GSButtonLayer::createShovelImage()
{
	mouseSelectImage->isSelectShovel = true;
	shovelImage = Sprite::create("resources/Images/System/Shovel_hi_res.png");
	shovelImage->setGlobalZOrder(1);
	shovelImage->setPosition(_shovelBank->getPosition() + Vec2(80, -40));
	this->addChild(shovelImage);
	updateShovel(false);
}

void GSButtonLayer::deleteShovelImage()
{
	mouseSelectImage->isSelectShovel = false;
	shovelImage->removeFromParent();
	updateShovel();
}

GLProgram* GSButtonLayer::greyScaleShader()
{
	auto program = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert,
		FileUtils::getInstance()->getStringFromFile("resources/Text/GreyScale.reanim.compiled").c_str());
	program->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
	program->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
	program->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
	program->link();
	program->updateUniforms();

	return program;
}

void GSButtonLayer::recoverySunControl()
{
	/* 子类中实现 */
}

void GSButtonLayer::recoveryCoinControl()
{
	/* 子类中实现 */
}
