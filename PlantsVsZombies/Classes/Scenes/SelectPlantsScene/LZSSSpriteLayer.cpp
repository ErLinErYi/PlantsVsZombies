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
#include "Based/LZBMouseEventControl.h"
#include "Plants/LZPPlants.h"
#include "../GameScene/LZSGData.h"
#include "../GameScene/LZSGAnimationLayer.h"

EventListenerMouse* SPSSpriteLayer::_listener = nullptr;
int SPSSpriteLayer::_listenerMutex = 0;
ui::ScrollView* SPSSpriteLayer::plantCardTextScrollView;
bool SPSSpriteLayer::isPlantIsCanSelect[]{};
Text* SPSSpriteLayer::animationText[]{};

SPSSpriteLayer::SPSSpriteLayer() :
	_seedChooser(nullptr)
,   _plantsAnimation(nullptr)
,	_plantCardScrollView(ScrollView::create())
,	_global(Global::getInstance())
,	_plantCardScrollViewPercent(0)
,	_selectFinished(false)
,   _plantsNumber(PLANTSNUMBERS)
{
	plantCardTextScrollView = ui::ScrollView::create();
	memset(isPlantIsCanSelect, true, sizeof(isPlantIsCanSelect));
}

SPSSpriteLayer::~SPSSpriteLayer()
{
	_plantsCards.clear();
	_listenerMutex = 0;
	memset(isPlantIsCanSelect, true, sizeof(isPlantIsCanSelect));
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
	_plantCardScrollView->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_plantCardScrollView->setContentSize(Size(760.0f, 420));
	_plantCardScrollView->setInnerContainerSize(Size(760, 525));
	_plantCardScrollView->setPosition(Vec2(0, 540));
	_plantCardScrollView->setBounceEnabled(true);
	_plantCardScrollView->setScrollBarPositionFromCorner(Vec2(5, 0));
	_plantCardScrollView->setScrollBarWidth(7);
	_plantCardScrollView->setScrollBarColor(Color3B(0,255,255));
	_plantCardScrollView->setScrollBarAutoHideTime(6666);
	_plantCardScrollView->scrollToTop(0.1f,true);
	_seedChooser->addChild(_plantCardScrollView);

	/* 植物卡牌介绍滑动视图 */
	plantCardTextScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	plantCardTextScrollView->setContentSize(Size(400.0f, 360.0f));
	plantCardTextScrollView->setPosition(Vec2(360, 580));
	plantCardTextScrollView->setBounceEnabled(true);
	plantCardTextScrollView->setScrollBarPositionFromCorner(Vec2(10, 0));
	plantCardTextScrollView->setScrollBarAutoHideTime(2.0f);
	plantCardTextScrollView->setScrollBarWidth(7);
	plantCardTextScrollView->setScrollBarColor(Color3B::WHITE);
	_seedChooser->addChild(plantCardTextScrollView);
}

void SPSSpriteLayer::createMouseListener()
{
	/* 鼠标监听 */
	_listener = EventListenerMouse::create();
	_listener->onMouseScroll = [&](Event* event)
	{
		auto e = (EventMouse*)event;
		auto cur = e->getLocationInView();
		cur.x -= 205;

		if (_plantCardScrollView->getBoundingBox().containsPoint(cur))
		{
			float movex = e->getScrollY() * 90;
			MouseEventControl::mouseScrollControlListener(
				_plantCardScrollView, movex, ScrollView::Direction::VERTICAL, 0.25f);
		}

		if (plantCardTextScrollView->getBoundingBox().containsPoint(cur))
		{
			float movex = e->getScrollY() * 90;
			MouseEventControl::mouseScrollControlListener(
				plantCardTextScrollView, movex, ScrollView::Direction::VERTICAL, 0.25f);
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
	for (unsigned int i = 0; i < 5; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			if (4 * i + j < _plantsNumber)
			{
				_plantsCards.push_back(
					createButtons(Vec2(110 + 185 * j, 490 - 105 * i), plantsCardInformation[4 * i + j].type));
			}
		}
	}
}

Button* SPSSpriteLayer::createButtons(const Vec2& vec2, PlantsType plantsType)
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
				if (isPlantIsCanSelect[static_cast<unsigned int>(plantsType)])
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

						createMoveButton(button, vec2, plantsType);
					}
				}

				createAnimationAndText(plantsType);//创建植物动画 
				
				break;
			}
		});
	_plantCardScrollView->addChild(button);

	controlPlantCanSelect(button, plantsType);              /* 控制植物是否可以选择 */
	showPlantsInformation(button, plantsType);              /* 显示植物信息 */
	createButtonHoverEffect(button);                        /* 鼠标悬停效果 */

	return button;
}

void SPSSpriteLayer::createButtonHoverEffect(Button* button)
{
	auto seedPacketFlash = Sprite::createWithSpriteFrameName("SeedPacketFlash.png");
	seedPacketFlash->setVisible(false);
	seedPacketFlash->setPosition(Vec2(0, -1));
	seedPacketFlash->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	seedPacketFlash->setName("seedPacketFlash");
	button->addChild(seedPacketFlash);
}

void SPSSpriteLayer::pauseButtonHoverEffect()
{
	++_listenerMutex;
	if (_listener)_listener->setEnabled(false);
}

void SPSSpriteLayer::resumeButtonHoverEffect()
{
	if (!--_listenerMutex && _listener)_listener->setEnabled(true);
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
			card->getBoundingBox().containsPoint(e->getLocationInView() - Vec2(205, calculateScrollDistance() + 15)));
	}
}

void SPSSpriteLayer::showRandPlantsInformation()
{
	preLoadText();

	/* 开始进入创建随机动画 */
	srand(time(nullptr));
	createAnimationAndText(static_cast<PlantsType>(rand() % _plantsNumber));
}

void SPSSpriteLayer::preLoadText()
{
	for (unsigned int i = 0; i < _plantsNumber; i++)
	{
		animationText[i] = Text::create("", GAME_FONT_NAME_1, 30);
		animationText[i]->setName("AnimationText");
		animationText[i]->enableOutline(Color4B(0, 255, 255, 255));
		animationText[i]->enableShadow(Color4B::GRAY, Size(4, -4), 1);
		switch (i)
		{
		case 0:
			_seedChooser->addChild(animationText[i]);
			break;
		default:
			plantCardTextScrollView->addChild(animationText[i]);
			break;
		}
	}
}

Text* SPSSpriteLayer::showPlantsInformation(Button* button, PlantsType type)
{
	/* 图片 */
	createPlantsImage(button, type);

	string str[] = { "PlantsLevel_Gold.png","PlantsLevel_Silver.png","PlantsLevel_Copper.png" };

	auto PlantsLevelIamge = Sprite::createWithSpriteFrameName(str[plantsCardInformation[static_cast<unsigned int>(type)].quality - 1]);
	PlantsLevelIamge->setPosition(Vec2(155, 74));
	PlantsLevelIamge->setScale(0.65f);
	button->addChild(PlantsLevelIamge);

	auto Background = Sprite::createWithSpriteFrameName("PlantsIconBackground.png");
	Background->setOpacity(0);
	button->addChild(Background);

	auto PlantsIconBackground = Sprite::createWithSpriteFrameName("PlantsIconBackground.png");
	PlantsIconBackground->setPosition(Vec2(97, 150));
	PlantsIconBackground->setScale(0.28f);
	PlantsIconBackground->setColor(plantsCardInformation[static_cast<unsigned int>(type)].color);
	Background->addChild(PlantsIconBackground);

	auto PlantsIcon = Sprite::createWithSpriteFrameName(plantsCardInformation[static_cast<unsigned int>(type)].information[1] + ".png");
	PlantsIcon->setPosition(Vec2(75, 75));
	PlantsIcon->setScale(1.2f);
	PlantsIconBackground->addChild(PlantsIcon);

	/* 文本 */
	auto PlantsNeedSuns = ui::Text::create();
	PlantsNeedSuns->setFontName(GAME_FONT_NAME_3);
	PlantsNeedSuns->setColor(Color3B::BLACK);
	PlantsNeedSuns->setAnchorPoint(Vec2(1, 0.5f));
	PlantsNeedSuns->setPosition(Vec2(175, 18));
	PlantsNeedSuns->setFontSize(35);
	PlantsNeedSuns->setString(to_string(plantsCardInformation[static_cast<unsigned int>(type)].plantsNeedSunNumbers));
	button->addChild(PlantsNeedSuns);

	auto PlantsLevelText = ui::Text::create();
	PlantsLevelText->setFontName(GAME_FONT_NAME_1);
	PlantsLevelText->setColor(Color3B::BLACK);
	PlantsLevelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	PlantsLevelText->setPosition(Vec2(153, 75));
	PlantsLevelText->setFontSize(25);
	PlantsLevelText->setString(plantsCardInformation[static_cast<unsigned int>(type)].information[2]);
	button->addChild(PlantsLevelText);

	if (!isPlantIsCanSelect[static_cast<unsigned int>(type)])
	{
		createProhibit(button);
	}

	return PlantsNeedSuns;
}

void SPSSpriteLayer::createPlantsImage(Button* button, PlantsType type, const float& scale) const
{
	auto image = Sprite::createWithSpriteFrameName(plantsCardInformation[static_cast<unsigned int>(type)].information[0] + ".png");
	image->setScale(scale);
	image->setFlippedX(plantsCardInformation[static_cast<unsigned int>(type)].flippedX);
	image->setPosition(Vec2(55, 50));
	button->addChild(image);
}

void SPSSpriteLayer::createMoveButton(Button* button, const Vec2& vec2, PlantsType plantsType)
{
	/* 创建移动卡牌 */
	auto moveCard = ui::Button::create("SeedPacket_Larger.png", "SeedPacket_Larger.png", "", TextureResType::PLIST);
	moveCard->setPosition(Vec2(vec2.x + 212, vec2.y + calculateScrollDistance()));
	moveCard->setTitleColor(Color3B::RED);
	moveCard->setTitleFontSize(25);
	moveCard->setTag(static_cast<unsigned int>(plantsType));
	moveCard->runAction(MoveTo::create(0.2f, Vec2(105, 1008 - 103 * seedBankButton.size())));
	this->addChild(moveCard);

	showPlantsInformation(moveCard, plantsType); // 显示信息
	createButtonHoverEffect(moveCard);           // 鼠标悬停信息

	/* 存储到卡牌栏中 */
	UserSelectCard seed_bank_button{};
	seed_bank_button.cardbutton = moveCard;
	seed_bank_button.cardTag = static_cast<unsigned int>(plantsType);
	seedBankButton.push_back(seed_bank_button);
	
	float plantCardRollingDistanceLast = calculateScrollDistance();
	moveCard->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap2");
				break;
			case ui::Widget::TouchEventType::ENDED:
				createAnimationAndText(plantsType);  //创建植物动画
				sortPlantsCard(plantsType);          //对植物卡牌重新排序
				_plantCardScrollView->scrollToPercentVertical(calculateScrollPrecent(plantCardRollingDistanceLast), 0.5f, true);//滚动到初始位置
				moveCard->runAction(Sequence::create(MoveTo::create(0.2f, Vec2(vec2.x + 212, vec2.y + plantCardRollingDistanceLast)),
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

float SPSSpriteLayer::calculateScrollDistance()
{
	_plantCardRollingDistance = (_plantCardScrollView->getInnerContainerSize().height -
		_plantCardScrollView->getContentSize().height) * _plantCardScrollView->getScrolledPercentVertical() / 100;
	return _plantCardRollingDistance;
}

float SPSSpriteLayer::calculateScrollPrecent(float distance)
{
	return distance * 100 / (_plantCardScrollView->getInnerContainerSize().height -
		_plantCardScrollView->getContentSize().height);
}

void SPSSpriteLayer::sortPlantsCard(PlantsType type)
{
	/* 删除取消选择的卡牌 */
	for (auto& card = seedBankButton.begin(); card != seedBankButton.end();)
	{
		if (card->cardTag == static_cast<unsigned int>(type))
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

void SPSSpriteLayer::controlPlantCanSelect(Button* button, PlantsType type)
{
	auto coinNumber = _global->userInformation->getCoinNumbers();
	auto killZombiesNumber = _global->userInformation->getKillZombiesNumbers();

	if (static_cast<unsigned int>(type) >= 9)
	{
		if (coinNumber < plantsCardInformation[static_cast<unsigned int>(type)].requirement.x ||
			killZombiesNumber < plantsCardInformation[static_cast<unsigned int>(type)].requirement.y)
		{
			button->setColor(Color3B(70, 70, 70));
			button->setCascadeColorEnabled(true);  /* 设置父节点影响子节点 */
			isPlantIsCanSelect[static_cast<unsigned int>(type)] = false;
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

void SPSSpriteLayer::createPlantsText(const unsigned int& ID, const std::string& name, const Vec2& vec2, const float& fontsize, Color3B color, bool AnchorPoint)
{
	animationText[ID]->setString(name);
	ID != 0 ? animationText[ID]->setTextAreaSize(Size(400, 800)) : nullptr;
	animationText[ID]->setFontSize(fontsize);
	animationText[ID]->setColor(color);
	plantCardTextScrollView->scrollToTop(0.1f, true);
	AnchorPoint ? animationText[ID]->setAnchorPoint(Vec2(0.5f, 0.5f)) : animationText[ID]->setAnchorPoint(Vec2(0, 1));
	switch (ID)
	{
	case 0:
		animationText[ID]->setPosition(vec2);
		break;
	default:
		animationText[ID]->setPosition(Vec2(
			vec2.x - plantCardTextScrollView->getInnerContainerSize().width + 40,
			vec2.y + plantCardTextScrollView->getInnerContainerSize().height - 1000));
		break;
	}
}

void SPSSpriteLayer::createAnimationAndText(PlantsType type)
{
	for (unsigned int i = 0; i < _plantsNumber; i++)animationText[i]->setString("");
	if (_plantsAnimation)_plantsAnimation->removeFromParent();

	auto plant = animationLayerInformation->createDifferentPlants(type);
	_plantsAnimation = plant->showPlantAnimationAndText();
	
	if (_plantsAnimation) _seedChooser->addChild(_plantsAnimation);
}

string SPSSpriteLayer::selectRequirementText(map<string, LanguageTextAttribute*>& lta, PlantsType type, string str, string str1)
{
	if (isPlantIsCanSelect[static_cast<unsigned int>(type)])
	{
		return lta.find(str)->second->text;
	}
	else
	{
		char buf[256];
		snprintf(buf, 256, lta.find(str1)->second->text.c_str(),
			plantsCardInformation[static_cast<unsigned int>(type)].requirement.x,
			plantsCardInformation[static_cast<unsigned int>(type)].requirement.y);
		string s(buf);
		return s;
	}
}
