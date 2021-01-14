/**LZBPlayMusic.h
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "LZSSSpriteLayer.h"
#include "spine/spine-cocos2dx.h"
#include "Based/LZBPlayMusic.h"

using namespace spine;

EventListenerMouse* SPSSpriteLayer::_listener = nullptr;

SPSSpriteLayer::SPSSpriteLayer() :
	_seedChooser(nullptr)
,	_plantCardScrollView(ScrollView::create())
,	_plantCardTextScrollView(ScrollView::create())
,	_global(Global::getInstance())
,	_plantCardScrollViewPercent(0)
,	_selectFinished(false)
,   _plantsNumber(13)
,   _isPlantIsCanSelect{ true,true,true,true,true,true,true,true,true,true,true,true ,true }
{
}

SPSSpriteLayer::~SPSSpriteLayer()
{
	_plantsCards.clear();
}

bool SPSSpriteLayer::init()
{
	if (!Layer::init())return false;

	createSelectPlantsDialog();

	return true;
}

void SPSSpriteLayer::createSelectPlantsDialog()
{
	PlayMusic::playMusic("swing");
	
	alreadySelectPlantsDialog();
	alreadyHavePlantsDialog();
	createScrollview();
	createPlantsCards();
	showRandPlantsInformation();
	createMouseListener();
}

void SPSSpriteLayer::alreadySelectPlantsDialog()
{
	auto seedBank = Scale9Sprite::createWithSpriteFrameName("SeedBank.png");
	seedBank->setPosition(Vec2(-200, 1080));
	seedBank->setAnchorPoint(Vec2(0, 1));
	//SeedBank->setCapInsets(Rect(0, 6, 87, 373));
	seedBank->setContentSize(Size(210, 967));
	seedBank->runAction(MoveTo::create(0.2f, Vec2(0, 1080)));
	this->addChild(seedBank);

	for (unsigned int i = 0; i < 9; ++i)
	{
		auto Seed = Sprite::createWithSpriteFrameName("SeedPacketSilhouette.png");
		Seed->setContentSize(Size(180, 100));
		Seed->setPosition(Vec2(105, 897 - 103 * i));
		seedBank->addChild(Seed);
	}
}

void SPSSpriteLayer::alreadyHavePlantsDialog()
{
	_seedChooser = Sprite::createWithSpriteFrameName("SeedChooser_Background.png");
	_seedChooser->setPosition(Vec2(595, -600));
	_seedChooser->setContentSize(Size(780, 1080));
	_seedChooser->setName("_seedChooer");
	_seedChooser->runAction(Sequence::create(MoveTo::create(0.2f, Vec2(595, 540)), CallFunc::create([&]() {createBeginButton(); }), nullptr));
	this->addChild(_seedChooser);

	auto SelectBackground = Sprite::createWithSpriteFrameName("SelectBackground.png");
	SelectBackground->setPosition(Vec2(190, 730));
	SelectBackground->setScaleY(0.96f);
	SelectBackground->setScaleX(1.14f);
	SelectBackground->setColor(Color3B(0, 255, 255));
	SelectBackground->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.2f, 150), FadeTo::create(0.2f, 50), nullptr)));
	_seedChooser->addChild(SelectBackground);

	auto State = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("Stage")->second);
	State->setPosition(Vec2(195, 720));
	State->setScale(3.5f);
	State->setAnimation(0, "Rotate", true);
	_seedChooser->addChild(State);

	auto SeedChooserText = ui::Text::create(_global->userInformation->getGameText().find("请选择你出战的植物！")->second->text, GAME_FONT_NAME_1,
		_global->userInformation->getGameText().find("请选择你出战的植物！")->second->fontsize);
	SeedChooserText->setColor(Color3B::YELLOW);
	SeedChooserText->setPosition(Vec2(400, 1000));
	_seedChooser->addChild(SeedChooserText);
}

void SPSSpriteLayer::createScrollview()
{
	/* 植物卡牌滑动视图 */
	_plantCardScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	_plantCardScrollView->setContentSize(Size(800.0f, 420));
	_plantCardScrollView->setPosition(Vec2(0, 120));
	_plantCardScrollView->setInnerContainerSize(Size(800, 420));
	//_plantCardScrollView->setBounceEnabled(true);
	_plantCardScrollView->setScrollBarPositionFromCorner(Vec2(40, 0));
	_plantCardScrollView->setScrollBarWidth(7);
	_plantCardScrollView->setScrollBarAutoHideTime(2.0f);
	_plantCardScrollView->setScrollBarColor(Color3B::WHITE);
	//this->schedule(schedule_selector(SPSSpriteLayer::scrollViewUpdate), 0.1f);
	_seedChooser->addChild(_plantCardScrollView);

	/* 植物卡牌介绍滑动视图 */
	_plantCardTextScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	_plantCardTextScrollView->setContentSize(Size(400.0f, 360.0f));
	_plantCardTextScrollView->setPosition(Vec2(360, 580));
	_plantCardTextScrollView->setBounceEnabled(true);
	_plantCardTextScrollView->setScrollBarPositionFromCorner(Vec2(10, 0));
	_plantCardTextScrollView->setScrollBarAutoHideTime(2.0f);
	_plantCardTextScrollView->setScrollBarWidth(7);
	_plantCardTextScrollView->setScrollBarColor(Color3B::WHITE);
	_seedChooser->addChild(_plantCardTextScrollView);
}

void SPSSpriteLayer::scrollViewUpdate(float Time)
{
	_plantCardScrollViewPercent = (_plantCardScrollView->getScrolledPercentVertical()) * 25;
}

void SPSSpriteLayer::createMouseListener()
{
	/* 鼠标监听 */
	_listener = EventListenerMouse::create();
	_listener->onMouseScroll = [&](Event* event)
	{
		auto e = (EventMouse*)event;
		auto cur = e->getLocationInView();
		auto a = e->getScrollY();
		cur.x -= 200;

		if (_plantCardScrollView->getBoundingBox().containsPoint(cur))
		{
			if (_plantCardRollingDistance + a >= 0 && _plantCardRollingDistance + a <= 50)
			{
				_plantCardRollingDistance += a;
			}
			if (_plantCardRollingDistance < 0)_plantCardRollingDistance = 0;
			if (_plantCardRollingDistance > 50)_plantCardRollingDistance = 50;
			_plantCardScrollView->jumpToPercentVertical(_plantCardRollingDistance * 2);
		}
	}; 

	_listener->onMouseMove = [&](Event* event)
	{
		EventMouse* e = (EventMouse*)event;

		showPopulationButtonHoverEffect(e);
		showSelectedButtonHoverEffect(e);
	};
	
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);
}

void SPSSpriteLayer::createPlantsCards()
{
	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			if (4 * i + j < _plantsNumber)
			{
				_plantsCards.push_back(createButtons(Vec2(110 + 185 * j, 490 - 105 * i), 4 * i + j));
			}
		}
	}
}

Button* SPSSpriteLayer::createButtons(const Vec2& vec2, const unsigned int& id)
{
	auto button = Button::create("SeedPacket_Larger.png", "SeedPacket_Larger.png", "", TextureResType::PLIST);
	button->setPosition(Vec2(vec2.x, _plantCardScrollView->getInnerContainerSize().height + vec2.y - 540));
	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap");
				break;
			case ui::Widget::TouchEventType::ENDED:
				if (_isPlantIsCanSelect[id])
				{
					if (seedBankButton.size() > 3)/* 583 */
					{
						((Button*)_seedChooser->getChildByName("Begin"))->setEnabled(true);
						((Button*)_seedChooser->getChildByName("Begin"))->setTitleColor(Color3B::YELLOW);
					}
					if (seedBankButton.size() < 9)
					{
						button->setEnabled(false);
						button->setColor(Color3B(70, 70, 70));
						button->setCascadeColorEnabled(true);  /* 设置父节点影响子节点 */

						createMoveButton(button, vec2, id);
					}
				}

				createAnimationAndText(id);//创建植物动画 
				
				break;
			}
		});
	_plantCardScrollView->addChild(button);

	createButtonHoverEffect(button);
	controlPlantCanSelect(button,id);               /* 控制植物是否可以选择 */
	showPlantsInformation(button, id);              /* 显示植物信息 */

	if (!_isPlantIsCanSelect[id])
	{
		createProhibit(button);
	}

	return button;
}

void SPSSpriteLayer::createButtonHoverEffect(Button* button)
{
	auto seedPacketFlash = Sprite::createWithSpriteFrameName("SeedPacketFlash.png");
	seedPacketFlash->setVisible(false);
	seedPacketFlash->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	seedPacketFlash->setName("seedPacketFlash");
	seedPacketFlash->setGlobalZOrder(1);
	button->addChild(seedPacketFlash);
}

void SPSSpriteLayer::pauseButtonHoverEffect()
{
	_listener->setEnabled(false);
}

void SPSSpriteLayer::resumeButtonHoverEffect()
{
	_listener->setEnabled(true);
}

void SPSSpriteLayer::showSelectedButtonHoverEffect(EventMouse* e)
{
	for (auto &card : seedBankButton)
	{
		card.cardbutton->getChildByName("seedPacketFlash")->setVisible(
			card.cardbutton->getBoundingBox().containsPoint(e->getLocationInView()));
	}
}

void SPSSpriteLayer::showPopulationButtonHoverEffect(EventMouse* e)
{
	for (auto card : _plantsCards)
	{
		card->getChildByName("seedPacketFlash")->setVisible(
			card->getBoundingBox().containsPoint(e->getLocationInView() - Vec2(205, 120)));
	}
}

void SPSSpriteLayer::showRandPlantsInformation()
{
	preLoadText();

	/* 开始进入创建随机动画 */
	srand(time(nullptr));
	createAnimationAndText(rand() % 11);
}

void SPSSpriteLayer::preLoadText()
{
	for (unsigned int i = 0; i < _plantsNumber; i++)
	{
		_animationText[i] = Text::create("", GAME_FONT_NAME_1, 30);
		_animationText[i]->setName("AnimationText");
		_animationText[i]->enableOutline(Color4B(0, 255, 255, 255));
		_animationText[i]->enableShadow(Color4B::GRAY, Size(4, -4), 1);
		switch (i)
		{
		case 0:
			_seedChooser->addChild(_animationText[i]);
			break;
		default:
			_plantCardTextScrollView->addChild(_animationText[i]);
			break;
		}
	}
}

Text* SPSSpriteLayer::showPlantsInformation(Button* button, const unsigned int id) const
{
	const string PlantsImageName[][5] =
	{
		{{"SunFlower_2"},{"50"},{"PlantsIcon4"}},{{"PeaShooter"},{"100"},{"PlantsIcon5"}},{{"Wallnut_body"},{"50"},{"PlantsIcon6"}},{{"CherryBomb"},{"150"},{"PlantsIcon7"}},
		{{"PotatoMine"},{"25"},{"PlantsIcon7"}},{{"Cabbage"},{"100"},{"PlantsIcon3"}},{{"Torchwood"},{"175"},{"PlantsIcon2"}},{{"Spikeweed"},{"100"},{"PlantsIcon12"}},
		{{"Garlic"},{"50"},{"PlantsIcon10"}},{{"FirePeaShooter"},{"200"},{"PlantsIcon2"}},{{"Jalapeno"},{"150"},{"PlantsIcon2"}},{{"LemonShooter"},{"125"},{"PlantsIcon10"}},
	    {{"Citron"},{"350"},{"PlantsIcon"}}
	};
	Color3B color[] =
	{
		Color3B::YELLOW,Color3B::GREEN,Color3B(140,80,10),Color3B::RED,Color3B(250,75,10),Color3B(64,0,0),Color3B::RED,Color3B::BLACK,Color3B(128,0,128),Color3B::RED,
		Color3B::RED,Color3B::ORANGE,Color3B(0,255,255)
	};

	/* 图片 */
	createPlantsImage(button, PlantsImageName[id][0]);

	auto PlantsLevelIamge = Sprite::createWithSpriteFrameName("PlantsLevel_Copper.png");
	PlantsLevelIamge->setPosition(Vec2(155, 74));
	PlantsLevelIamge->setScale(0.65f);
	button->addChild(PlantsLevelIamge);

	auto Background = Sprite::createWithSpriteFrameName("PlantsIconBackground.png");
	Background->setOpacity(0);
	button->addChild(Background);

	auto PlantsIconBackground = Sprite::createWithSpriteFrameName("PlantsIconBackground.png");
	PlantsIconBackground->setPosition(Vec2(97, 150));
	PlantsIconBackground->setScale(0.28f);
	PlantsIconBackground->setColor(color[id]);
	Background->addChild(PlantsIconBackground);

	auto PlantsIcon = Sprite::createWithSpriteFrameName(PlantsImageName[id][2] + ".png");
	PlantsIcon->setPosition(Vec2(75, 75));
	PlantsIcon->setScale(1.2f);
	PlantsIconBackground->addChild(PlantsIcon);

	/* 文本 */
	auto PlantsNeedSuns = ui::Text::create();
	PlantsNeedSuns->setFontName(GAME_FONT_NAME_2);
	PlantsNeedSuns->setColor(Color3B::BLACK);
	PlantsNeedSuns->setAnchorPoint(Vec2(1, 0.5f));
	PlantsNeedSuns->setPosition(Vec2(175, 20));
	PlantsNeedSuns->setFontSize(30);
	PlantsNeedSuns->setString(PlantsImageName[id][1]);
	button->addChild(PlantsNeedSuns);

	auto PlantsLevelText = ui::Text::create();
	PlantsLevelText->setFontName(GAME_FONT_NAME_1);
	PlantsLevelText->setColor(Color3B::BLACK);
	PlantsLevelText->setAnchorPoint(Vec2(1, 0.5f));
	PlantsLevelText->setPosition(Vec2(177, 77));
	PlantsLevelText->setFontSize(20);
	PlantsLevelText->setString("LVL1");
	button->addChild(PlantsLevelText);

	return PlantsNeedSuns;
}

void SPSSpriteLayer::createPlantsImage(Button* button, const std::string& resource, const float& scale) const
{
	auto image = Sprite::createWithSpriteFrameName(resource + ".png");
	image->setScale(scale);
	image->setPosition(Vec2(55, 50));
	button->addChild(image);
}

void SPSSpriteLayer::createMoveButton(Button* button, const Vec2& vec2, const unsigned int& id)
{
	/* 创建移动卡牌 */
	auto moveCard = ui::Button::create("SeedPacket_Larger.png", "SeedPacket_Larger.png", "", TextureResType::PLIST);
	moveCard->setPosition(Vec2(vec2.x + 212, vec2.y + _plantCardScrollViewPercent));
	moveCard->setTitleColor(Color3B::RED);
	moveCard->setTitleFontSize(25);
	moveCard->setTag(id);
	moveCard->runAction(MoveTo::create(0.2f, Vec2(105, 1008 - 103 * seedBankButton.size())));
	this->addChild(moveCard);

	createButtonHoverEffect(moveCard);

	showPlantsInformation(moveCard, id); //显示信息

	/* 存储到卡牌栏中 */
	UserSelectCard seed_bank_button{};
	seed_bank_button.cardbutton = moveCard;
	seed_bank_button.cardTag = id;
	seedBankButton.push_back(seed_bank_button);
	
	float PlantCardScrollViewPercentLast = _plantCardScrollViewPercent;
	moveCard->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap2");
				break;
			case ui::Widget::TouchEventType::ENDED:
				createAnimationAndText(id);  //创建植物动画
				sortPlantsCard(id);          //对植物卡牌重新排序
				_plantCardScrollView->scrollToPercentVertical(PlantCardScrollViewPercentLast / 25.0f, 0.5f, true);//滚动到初始位置
				_plantCardRollingDistance = PlantCardScrollViewPercentLast / 25.0f;
				moveCard->runAction(Sequence::create(MoveTo::create(0.2f, Vec2(vec2.x + 212, vec2.y + PlantCardScrollViewPercentLast)),
					CallFunc::create([=]()
						{
							moveCard->removeFromParent();
							
							/* 设置卡牌精灵可以再次选择 */
							button->setEnabled(true);
							button->setColor(Color3B::WHITE);
						}), nullptr));//植物卡牌取消选择
				break;
			}
		});
}

void SPSSpriteLayer::sortPlantsCard(const unsigned int& id)
{
	/* 删除取消选择的卡牌 */
	for (auto& card = seedBankButton.begin(); card != seedBankButton.end();)
	{
		if (card->cardTag == id)
		{
			card = seedBankButton.erase(card);
		}
		else
		{
			++card;
		}
	}

	/* 为植物卡牌重新设置位置 */
	int i = -1;
	for (auto& card : seedBankButton)
	{
		card.cardbutton->runAction(MoveTo::create(0.2f, Vec2(105, 1008 - 103 * ++i)));
	}

	if (seedBankButton.size() <= 4)
	{
		((Button*)_seedChooser->getChildByName("Begin"))->setEnabled(false);
		((Button*)_seedChooser->getChildByName("Begin"))->setTitleColor(Color3B::WHITE);
	}
}

void SPSSpriteLayer::createBeginButton()
{
	auto button = Button::create("SeedChooser_Button_Disabled.png", "SeedChooser_Button.png", "", TextureResType::PLIST);
	button->setTitleText(_global->userInformation->getGameText().find("开始战斗吧！")->second->text);
	button->setTitleColor(Color3B::WHITE);
	button->setTitleFontName(GAME_FONT_NAME_1);
	button->setTitleFontSize(_global->userInformation->getGameText().find("开始战斗吧！")->second->fontsize);
	button->setPosition(Vec2(390, 70));
	button->setScale(2.0f);
	button->setEnabled(false);
	button->setName("Begin");
	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap");
				break;
			case ui::Widget::TouchEventType::ENDED:
				startGame();
				break;
			}
		});
	_seedChooser->addChild(button);
}

void SPSSpriteLayer::startGame()
{
	PlayMusic::playMusic("swing");
	/* 设置按钮不可用 */
	for (auto& card : seedBankButton)
	{
		card.cardbutton->setEnabled(false);
		card.cardbutton->setColor(Color3B::GRAY);
		card.cardbutton->setCascadeColorEnabled(true);
	}
	if (seedBankButton.size() > 4)
	{
		_seedChooser->runAction(Sequence::create(MoveTo::create(0.35f, Vec2(605, -600)),
			CallFuncN::create(CC_CALLBACK_1(SPSSpriteLayer::selectPlantsCallBack, this)), nullptr));
	}

	pauseButtonHoverEffect();
}

void SPSSpriteLayer::controlPlantCanSelect(Button* button, const unsigned int id)
{
	auto coinNumber = _global->userInformation->getCoinNumbers();
	auto killZombiesNumber = _global->userInformation->getKillZombiesNumbers();

	if (id >= 9)
	{
		if (coinNumber < requirement[id - 9].x || killZombiesNumber < requirement[id - 9].y)
		{
			button->setColor(Color3B(70, 70, 70));
			button->setCascadeColorEnabled(true);  /* 设置父节点影响子节点 */
			_isPlantIsCanSelect[id] = false;
		}
	}
}

void SPSSpriteLayer::createProhibit(Button* button)
{
	auto prohibit = Sprite::createWithSpriteFrameName("Prohibit.png");
	prohibit->setPosition(Vec2(160, 20));
	prohibit->setScale(0.5f);
	button->addChild(prohibit);
}

void SPSSpriteLayer::createPlantsAnimation(const std::string& filepath, const std::string& AnimationName, const std::string& skin, Vec2& vec2, const float& scale)
{
	/* 创建动画 */
	auto iter = _global->userInformation->getAnimationData().find(filepath);
	if (iter != _global->userInformation->getAnimationData().end())/* 如果可以找到 */
	{
		_plantsAnimation = SkeletonAnimation::createWithData(iter->second);
		_plantsAnimation->setPosition(vec2);
		_plantsAnimation->setScale(scale);
		_plantsAnimation->setSkin(skin);
		_plantsAnimation->setAnimation(0, AnimationName, true);
		_plantsAnimation->setName("PlantsAnimation");
	}
}

void SPSSpriteLayer::createPlantsText(const unsigned int& ID, const std::string& name, const Vec2& vec2, const float& fontsize, Color3B color, bool AnchorPoint)
{
	_animationText[ID]->setString(name);
	ID != 0 ? _animationText[ID]->setTextAreaSize(Size(400, 800)) : nullptr;
	_animationText[ID]->setFontSize(fontsize);
	_animationText[ID]->setColor(color);
	_plantCardTextScrollView->scrollToTop(0.1f, true);
	AnchorPoint ? _animationText[ID]->setAnchorPoint(Vec2(0.5f, 0.5f)) : _animationText[ID]->setAnchorPoint(Vec2(0, 1));
	switch (ID)
	{
	case 0:
		_animationText[ID]->setPosition(vec2);
		break;
	default:
		_animationText[ID]->setPosition(Vec2(vec2.x - _plantCardTextScrollView->getInnerContainerSize().width + 40, vec2.y + _plantCardTextScrollView->getInnerContainerSize().height - 1000));
		break;
	}
}

void SPSSpriteLayer::createAnimationAndText(const unsigned int& id)
{
	_seedChooser->removeChildByName("PlantsAnimation");
	for (unsigned int i = 0; i < _plantsNumber; i++)_animationText[i]->setString("");

	auto& lta = _global->userInformation->getGameText();
	switch (id)
	{
	case 0:
		_plantCardTextScrollView->setInnerContainerSize(Size(lta.find("SUNFLOWER_1")->second->position));
		this->createPlantsAnimation("SunFlower_2", "SunFlower_Normal", "SunFlower_Normal", Vec2(200, 610), 1.5f);
		this->createPlantsText(0, lta.find("SUNFLOWER_1")->second->text, Vec2(190, 910), lta.find("SUNFLOWER_1")->second->fontsize);
		this->createPlantsText(1, lta.find("SUNFLOWER_2")->second->text, Vec2(360, 1000), lta.find("SUNFLOWER_2")->second->fontsize, Color3B::YELLOW, false);
		this->createPlantsText(2, lta.find("SUNFLOWER_3")->second->text, Vec2(440, 1000), lta.find("SUNFLOWER_3")->second->fontsize, Color3B::RED, false);
		this->createPlantsText(3, lta.find("SUNFLOWER_4")->second->text, Vec2(360, 830), lta.find("SUNFLOWER_4")->second->fontsize, Color3B::YELLOW, false);
		break;
	case 1:
		_plantCardTextScrollView->setInnerContainerSize(Size(lta.find("PEASHOOTER_1")->second->position));
		this->createPlantsAnimation("PeaShooter", "PeaShooter_Normal", "", Vec2(200, 610), 1.8f);
		this->createPlantsText(0, lta.find("PEASHOOTER_1")->second->text, Vec2(190, 910), lta.find("PEASHOOTER_1")->second->fontsize);
		this->createPlantsText(2, lta.find("PEASHOOTER_2")->second->text, Vec2(360, 1000), lta.find("PEASHOOTER_2")->second->fontsize, Color3B::YELLOW, false);
		this->createPlantsText(3, lta.find("PEASHOOTER_3")->second->text, Vec2(440, 1000), lta.find("PEASHOOTER_3")->second->fontsize, Color3B::RED, false);
		this->createPlantsText(1, lta.find("PEASHOOTER_4")->second->text, Vec2(360, 830), lta.find("PEASHOOTER_4")->second->fontsize, Color3B::YELLOW, false);

		break;
	case 2:
		_plantCardTextScrollView->setInnerContainerSize(Size(lta.find("WALLNUT_1")->second->position));
		this->createPlantsAnimation("WallNut", "WallNut_Nornal", "WallNut_Normal", Vec2(200, 610), 1.5f);
		this->createPlantsText(0, lta.find("WALLNUT_1")->second->text, Vec2(190, 910), lta.find("WALLNUT_1")->second->fontsize);
		this->createPlantsText(2, lta.find("WALLNUT_2")->second->text, Vec2(360, 1000), lta.find("WALLNUT_2")->second->fontsize, Color3B::YELLOW, false);
		this->createPlantsText(3, lta.find("WALLNUT_3")->second->text, Vec2(440, 1000), lta.find("WALLNUT_3")->second->fontsize, Color3B::RED, false);
		this->createPlantsText(1, lta.find("WALLNUT_4")->second->text, Vec2(360, 870), lta.find("WALLNUT_4")->second->fontsize, Color3B::YELLOW, false);
		break;
	case 3:
		_plantCardTextScrollView->setInnerContainerSize(Size(lta.find("CHERRYBOMB_1")->second->position));
		this->createPlantsAnimation("CherryBomb", "CherryBomb_Normal", "", Vec2(200, 610), 1.8f);
		this->createPlantsText(0, lta.find("CHERRYBOMB_1")->second->text, Vec2(190, 910), lta.find("CHERRYBOMB_1")->second->fontsize);
		this->createPlantsText(2, lta.find("CHERRYBOMB_2")->second->text, Vec2(360, 1000), lta.find("CHERRYBOMB_2")->second->fontsize, Color3B::YELLOW, false);
		this->createPlantsText(3, lta.find("CHERRYBOMB_3")->second->text, Vec2(440, 1000), lta.find("CHERRYBOMB_3")->second->fontsize, Color3B::RED, false);
		this->createPlantsText(1, lta.find("CHERRYBOMB_4")->second->text, Vec2(360, 870), lta.find("CHERRYBOMB_4")->second->fontsize, Color3B::YELLOW, false);
		break;
	case 4:
		_plantCardTextScrollView->setInnerContainerSize(Size(lta.find("POTATOMINE_1")->second->position));
		this->createPlantsAnimation("PotatoMine", "PotatoMine_Normal", "", Vec2(200, 610), 1.8f);
		this->createPlantsText(0, lta.find("POTATOMINE_1")->second->text, Vec2(190, 910), lta.find("POTATOMINE_1")->second->fontsize);
		this->createPlantsText(2, lta.find("POTATOMINE_2")->second->text, Vec2(360, 1000), lta.find("POTATOMINE_2")->second->fontsize, Color3B::YELLOW, false);
		this->createPlantsText(3, lta.find("POTATOMINE_3")->second->text, Vec2(440, 1000), lta.find("POTATOMINE_3")->second->fontsize, Color3B::RED, false);
		this->createPlantsText(1, lta.find("POTATOMINE_4")->second->text, Vec2(360, 870), lta.find("POTATOMINE_4")->second->fontsize, Color3B::YELLOW, false);
		break;
	case 5:
		_plantCardTextScrollView->setInnerContainerSize(Size(lta.find("CABBAGE_1")->second->position));
		this->createPlantsAnimation("Cabbage", "Cabbage_Normal", "", Vec2(200, 610), 1.3f);
		this->createPlantsText(0, lta.find("CABBAGE_1")->second->text, Vec2(190, 910), lta.find("CABBAGE_1")->second->fontsize);
		this->createPlantsText(2, lta.find("CABBAGE_2")->second->text, Vec2(360, 1000), lta.find("CABBAGE_2")->second->fontsize, Color3B::YELLOW, false);
		this->createPlantsText(3, lta.find("CABBAGE_3")->second->text, Vec2(440, 1000), lta.find("CABBAGE_3")->second->fontsize, Color3B::RED, false);
		this->createPlantsText(1, lta.find("CABBAGE_4")->second->text, Vec2(360, 870), lta.find("CABBAGE_4")->second->fontsize, Color3B::YELLOW, false);
		break;
	case 6:
		_plantCardTextScrollView->setInnerContainerSize(Size(lta.find("TORCHWOOD_1")->second->position));
		this->createPlantsAnimation("Torchwood", "animation", "", Vec2(200, 610), 1.8f);
		this->createPlantsText(0, lta.find("TORCHWOOD_1")->second->text, Vec2(190, 910), lta.find("TORCHWOOD_1")->second->fontsize);
		this->createPlantsText(2, lta.find("TORCHWOOD_2")->second->text, Vec2(360, 1000), lta.find("TORCHWOOD_2")->second->fontsize, Color3B::YELLOW, false);
		this->createPlantsText(3, lta.find("TORCHWOOD_3")->second->text, Vec2(440, 1000), lta.find("TORCHWOOD_3")->second->fontsize, Color3B::RED, false);
		this->createPlantsText(1, lta.find("TORCHWOOD_4")->second->text, Vec2(360, 870), lta.find("TORCHWOOD_4")->second->fontsize, Color3B::YELLOW, false);
		break;
	case 7:
		_plantCardTextScrollView->setInnerContainerSize(Size(lta.find("SPIKEWEED_1")->second->position));
		this->createPlantsAnimation("Spikeweed", "Spikeweed_Normal", "", Vec2(200, 610), 1.5f);
		this->createPlantsText(0, lta.find("SPIKEWEED_1")->second->text, Vec2(190, 910), lta.find("SPIKEWEED_1")->second->fontsize);
		this->createPlantsText(2, lta.find("SPIKEWEED_2")->second->text, Vec2(360, 1000), lta.find("SPIKEWEED_2")->second->fontsize, Color3B::YELLOW, false);
		this->createPlantsText(3, lta.find("SPIKEWEED_3")->second->text, Vec2(440, 1000), lta.find("SPIKEWEED_3")->second->fontsize, Color3B::RED, false);
		this->createPlantsText(1, lta.find("SPIKEWEED_4")->second->text, Vec2(360, 870), lta.find("SPIKEWEED_4")->second->fontsize, Color3B::YELLOW, false);
		break;
	case 8:
		_plantCardTextScrollView->setInnerContainerSize(Size(lta.find("GARLIC_1")->second->position));
		this->createPlantsAnimation("Garlic", "Garlic_Normal", "", Vec2(200, 610), 1.5f);
		this->createPlantsText(0, lta.find("GARLIC_1")->second->text, Vec2(190, 910), lta.find("GARLIC_1")->second->fontsize);
		this->createPlantsText(2, lta.find("GARLIC_2")->second->text, Vec2(360, 1000), lta.find("GARLIC_2")->second->fontsize, Color3B::YELLOW, false);
		this->createPlantsText(3, lta.find("GARLIC_3")->second->text, Vec2(440, 1000), lta.find("GARLIC_3")->second->fontsize, Color3B::RED, false);
		this->createPlantsText(1, lta.find("GARLIC_4")->second->text, Vec2(360, 870), lta.find("GARLIC_4")->second->fontsize, Color3B::YELLOW, false);
		break;
	case 9:
		_plantCardTextScrollView->setInnerContainerSize(Size(lta.find("FIREPEASHOOTER_1")->second->position));
		this->createPlantsAnimation("FirePeaShooter", "FirePeaShooter_Normal", "", Vec2(200, 610), 1.8f);
		this->createPlantsText(0, lta.find("FIREPEASHOOTER_1")->second->text, Vec2(190, 910), lta.find("FIREPEASHOOTER_1")->second->fontsize);
		this->createPlantsText(2, lta.find("FIREPEASHOOTER_2")->second->text, Vec2(360, 1000), lta.find("FIREPEASHOOTER_2")->second->fontsize, Color3B::YELLOW, false);
		this->createPlantsText(3, lta.find("FIREPEASHOOTER_3")->second->text, Vec2(440, 1000), lta.find("FIREPEASHOOTER_3")->second->fontsize, Color3B::RED, false);
		this->createPlantsText(1, selectRequirementText(lta, 9, "FIREPEASHOOTER_4", "FIREPEASHOOTER_5"), Vec2(360, 870),
			lta.find("FIREPEASHOOTER_4")->second->fontsize, _isPlantIsCanSelect[9] ? Color3B::YELLOW : Color3B(255, 70, 0), false);
		break;
	case 10:
		_plantCardTextScrollView->setInnerContainerSize(Size(lta.find("JALAPENO_1")->second->position));
		this->createPlantsAnimation("Jalapeno", "Jalapeno_Normal", "", Vec2(200, 610), 1.5f);
		this->createPlantsText(0, lta.find("JALAPENO_1")->second->text, Vec2(190, 910), lta.find("JALAPENO_1")->second->fontsize);
		this->createPlantsText(2, lta.find("JALAPENO_2")->second->text, Vec2(360, 1000), lta.find("JALAPENO_2")->second->fontsize, Color3B::YELLOW, false);
		this->createPlantsText(3, lta.find("JALAPENO_3")->second->text, Vec2(440, 1000), lta.find("JALAPENO_3")->second->fontsize, Color3B::RED, false);
		this->createPlantsText(1, selectRequirementText(lta, 10, "JALAPENO_4", "JALAPENO_5"), Vec2(360, 870),
			lta.find("JALAPENO_4")->second->fontsize, _isPlantIsCanSelect[10] ? Color3B::YELLOW : Color3B(255, 70, 0), false);
		break;
	case 11:
		_plantCardTextScrollView->setInnerContainerSize(Size(lta.find("ACIDLEMON_1")->second->position));
		this->createPlantsAnimation("LemonShooter", "LemonNormal", "", Vec2(200, 610), 2.2f);
		this->createPlantsText(0, lta.find("ACIDLEMON_1")->second->text, Vec2(190, 910), lta.find("ACIDLEMON_1")->second->fontsize);
		this->createPlantsText(2, lta.find("ACIDLEMON_2")->second->text, Vec2(360, 1000), lta.find("ACIDLEMON_2")->second->fontsize, Color3B::YELLOW, false);
		this->createPlantsText(3, lta.find("ACIDLEMON_3")->second->text, Vec2(440, 1000), lta.find("ACIDLEMON_3")->second->fontsize, Color3B::RED, false);
		this->createPlantsText(1, selectRequirementText(lta, 11, "ACIDLEMON_4", "ACIDLEMON_5"), Vec2(360, 870),
			lta.find("ACIDLEMON_4")->second->fontsize, _isPlantIsCanSelect[11] ? Color3B::YELLOW : Color3B(255, 70, 0), false);
		break;
	case 12:
		_plantCardTextScrollView->setInnerContainerSize(Size(lta.find("CITRON_1")->second->position));
		this->createPlantsAnimation("Citron", "normal", "", Vec2(200, 610), 2.0f);
		this->createPlantsText(0, lta.find("CITRON_1")->second->text, Vec2(190, 910), lta.find("CITRON_1")->second->fontsize);
		this->createPlantsText(2, lta.find("CITRON_2")->second->text, Vec2(360, 1000), lta.find("CITRON_2")->second->fontsize, Color3B::YELLOW, false);
		this->createPlantsText(3, lta.find("CITRON_3")->second->text, Vec2(440, 1000), lta.find("CITRON_3")->second->fontsize, Color3B::RED, false);
		this->createPlantsText(1, selectRequirementText(lta, 12, "CITRON_4", "CITRON_5"), Vec2(360, 870),
			lta.find("CITRON_4")->second->fontsize, _isPlantIsCanSelect[12] ? Color3B::YELLOW : Color3B(255, 70, 0), false);
		break;
	default:
		break;
	}
	_seedChooser->addChild(_plantsAnimation);
}

string SPSSpriteLayer::selectRequirementText(map<string, LanguageTextAttribute*>& lta, int id, string str, string str1)
{
	if (_isPlantIsCanSelect[id])
	{
		return lta.find(str)->second->text;
	}
	else
	{
		char buf[256];
		snprintf(buf, 256, lta.find(str1)->second->text.c_str(), requirement[id - 9].x, requirement[id - 9].y);
		string s(buf);
		return s;
	}
}