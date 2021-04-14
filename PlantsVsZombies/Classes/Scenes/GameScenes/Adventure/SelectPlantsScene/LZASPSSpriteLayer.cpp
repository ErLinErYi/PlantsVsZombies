/**LZBPlayMusic.h
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "spine/spine-cocos2dx.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZMouseEventControl.h"
#include "Based/LZUserData.h"
#include "Plants/LZPlants.h"
#include "LZASPSSpriteLayer.h"
#include "LZASPSPlantSCardUnlockLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"

EventListenerMouse* SPSSpriteLayer::_listener = nullptr;
int SPSSpriteLayer::_listenerMutex = 0;
ui::ScrollView* SPSSpriteLayer::plantCardTextScrollView;
bool SPSSpriteLayer::isPlantIsCanSelect[]{};
Text* SPSSpriteLayer::animationText[]{};

SPSSpriteLayer::SPSSpriteLayer() :
	_seedChooser(nullptr)
,   _plantsAnimation(nullptr)
,   _plant(nullptr)
,	_plantCardScrollView(ScrollView::create())
,	_global(Global::getInstance())
,	_plantCardScrollViewPercent(0)
,	_selectFinished(false)
{
	plantCardTextScrollView = ui::ScrollView::create();
	memset(isPlantIsCanSelect, true, sizeof(isPlantIsCanSelect));
	PlantsCardUnlock::unlockPlantsCard.clear();
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
	popUnlockDialog();
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

	auto selectBackground = Sprite::createWithSpriteFrameName("SelectBackground.png");
	selectBackground->setPosition(Vec2(190, 730));
	selectBackground->setScaleY(0.96f);
	selectBackground->setScaleX(1.14f);
	selectBackground->setColor(Color3B(0, 255, 255));
	selectBackground->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.2f, 150), FadeTo::create(0.2f, 50), nullptr)));
	_seedChooser->addChild(selectBackground);

	auto state = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("Stage")->second);
	state->setAnimation(0, "Rotate", true);
	state->setPosition(Vec2(195, 720));
	state->setScale(3.5f);
	state->update(0);
	_seedChooser->addChild(state);

	auto bufEffect = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("BufEffect")->second);
	bufEffect->setAnimation(0, "animation", true);
	bufEffect->setPosition(Vec2(195, 520));
	bufEffect->setScale(1.5f);
	bufEffect->update(0);
	_seedChooser->addChild(bufEffect, 1);

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
	_plantCardScrollView->setInnerContainerSize(Size(760, 105 * ceil(PLANTSNUMBERS / 4.f)));
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
		const auto e = (EventMouse*)event;
		auto cur = e->getLocationInView();
		cur.x -= 205;

		if (_plantCardScrollView->getBoundingBox().containsPoint(cur))
		{
			const float movex = e->getScrollY() * 60;
			MouseEventControl::mouseScrollControlListener(
				_plantCardScrollView, movex, ScrollView::Direction::VERTICAL, 0.25f);
		}

		if (plantCardTextScrollView->getBoundingBox().containsPoint(cur))
		{
			const float movex = e->getScrollY() * 90;
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
	const auto number = static_cast<unsigned int>(ceil(PLANTSNUMBERS / 4.f));
	for (unsigned int i = 0; i < number; ++i)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			if (4 * i + j < PLANTSNUMBERS)
			{
				auto card = createButtons(Vec2(110 + 185 * j, 105 * i), 4 * i + j);
				_plantsCards.insert(pair<PlantsType, Button*>(plantsCardInformation[4 * i + j].type, card));

				controlPlantCanSelect(card, 4 * i + j);              /* 控制植物是否可以选择 */
				showPlantsInformation(card, true);                   /* 显示植物信息 */
				createButtonHoverEffect(card);                       /* 鼠标悬停效果 */
			}
		}
	}
}

Button* SPSSpriteLayer::createButtons(const Vec2& vec2, int priority)
{
	auto imageBg = "SeedPacket" + to_string(plantsCardInformation[priority].quality) + ".png";
	auto button = Button::create(imageBg, imageBg, "", TextureResType::PLIST);
	button->setPosition(Vec2(vec2.x, _plantCardScrollView->getInnerContainerSize().height - button->getContentSize().height / 2.f - vec2.y));
	button->setTag(priority);
	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::ENDED:
				PlayMusic::playMusic("tap");
				plantsCardButtonEvent(button, vec2);
				break;
			}
		});
	_plantCardScrollView->addChild(button);

	return button;
}

void SPSSpriteLayer::plantsCardButtonEvent(Button* button, Vec2 vec2)
{
	if (isPlantIsCanSelect[static_cast<unsigned int>(plantsCardInformation[button->getTag()].type)])
	{
		if (seedBankButton.size() > 4)/* 583 */
		{
			((Button*)_seedChooser->getChildByName("Begin"))->setEnabled(true);
			((Button*)_seedChooser->getChildByName("Begin"))->setTitleColor(Color3B::YELLOW);
		}
		if (seedBankButton.size() < 9)
		{
			button->setEnabled(false);
			button->setColor(Color3B(70, 70, 70));
			button->setCascadeColorEnabled(true);  /* 设置父节点影响子节点 */

			createMoveButton(button, vec2);
		}
	}
	createAnimationAndText(plantsCardInformation[button->getTag()].type);//创建植物动画 
}

void SPSSpriteLayer::createMoveButton(Button* button, const Vec2& vec2)
{
	/* 创建移动卡牌 */
	auto imageBg = "SeedPacket" + to_string(plantsCardInformation[button->getTag()].quality) + ".png";;
	auto moveCard = ui::Button::create(imageBg, imageBg, "", TextureResType::PLIST);
	moveCard->setPosition(Vec2(vec2.x + 205, _plantCardScrollView->getInnerContainerSize().
		height - moveCard->getContentSize().height - vec2.y - 40 + calculateScrollDistance()));
	moveCard->setTitleColor(Color3B::RED);
	moveCard->setTitleFontSize(25);
	moveCard->setTag(button->getTag());
	moveCard->setEnabled(false);
	moveCard->setBright(true);
	this->addChild(moveCard);
	moveCard->runAction(Sequence::create(
		MoveTo::create(0.35f, Vec2(105, 1008 - 103 * seedBankButton.size())), 
		CallFunc::create([moveCard]() {moveCard->setEnabled(true); }), nullptr));

	showPlantsInformation(moveCard);   // 显示信息
	createButtonHoverEffect(moveCard); // 鼠标悬停信息

	/* 存储到卡牌栏中 */
	UserSelectCard seed_bank_button{};
	seed_bank_button.cardbutton = moveCard;
	seed_bank_button.cardTag = button->getTag();
	seedBankButton.push_back(seed_bank_button);

	const float plantCardRollingDistanceLast = calculateScrollDistance();
	moveCard->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::ENDED: 
				PlayMusic::playMusic("tap2");
				plantsMoveCardButtonEvent(button, moveCard, vec2, plantCardRollingDistanceLast);
				break;
			}
		});
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

void SPSSpriteLayer::plantsMoveCardButtonEvent(Button* button, Button* moveCard, Vec2 vec2, float plantCardRollingDistanceLast)
{
	createAnimationAndText(plantsCardInformation[button->getTag()].type);  //创建植物动画
	sortPlantsCard(plantsCardInformation[button->getTag()].type);          //对植物卡牌重新排序
	_plantCardScrollView->scrollToPercentVertical(calculateScrollPrecent(plantCardRollingDistanceLast), 0.5f, true);//滚动到初始位置
	moveCard->runAction(Sequence::create(MoveTo::create(0.2f, Vec2(vec2.x + 205, _plantCardScrollView->getInnerContainerSize().height -
		moveCard->getContentSize().height - vec2.y - 40 + plantCardRollingDistanceLast)),
		CallFunc::create([=]()
			{
				moveCard->removeFromParent();   /* 删除移动卡牌 */
				button->setEnabled(true);       /* 设置卡牌精灵可以再次选择 */
				button->setColor(Color3B::WHITE);
			}), nullptr));
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

void SPSSpriteLayer::setListenerMutexNumber(int number)
{
	_listenerMutex = number;
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
		card.second->getChildByName("seedPacketFlash")->setVisible(
			card.second->getBoundingBox().containsPoint(e->getLocationInView() - Vec2(205, calculateScrollDistance() - 90)));
	}
}

void SPSSpriteLayer::showRandPlantsInformation()
{
	preLoadText();
	createAnimationAndText(plantsCardInformation[rand() % PLANTSNUMBERS].type);
}

void SPSSpriteLayer::preLoadText()
{
	for (unsigned int i = 0; i < 4; i++)
	{
		animationText[i] = Text::create("", GAME_FONT_NAME_1, 30);
		animationText[i]->setName("AnimationText");
		animationText[i]->enableShadow(Color4B::GRAY, Size(2, -2), 1);
		animationText[i]->enableOutline(Color4B(0, 255, 255, 255));
		animationText[i]->setLineBreakWithoutSpace(true);
		switch (i)
		{
		case 0:  _seedChooser->addChild(animationText[i]);            break;
		default: 
			animationText[i]->setTextAreaSize(Size(395, 800));
			plantCardTextScrollView->addChild(animationText[i]);      break;
		}
	}
}

Text* SPSSpriteLayer::showPlantsInformation(Button* button, bool showHeart)
{
	/* 图片 */
	auto plantImage = Sprite::createWithSpriteFrameName(plantsCardInformation[button->getTag()].information[0] + ".png");
	plantImage->setScale(0.8f);
	plantImage->setFlippedX(plantsCardInformation[button->getTag()].flippedX);
	plantImage->setPosition(Vec2(55, 50));
	button->addChild(plantImage);

	string str[] = { "PlantsLevel_Gold.png","PlantsLevel_Silver.png","PlantsLevel_Copper.png" };
	auto plantsLevelIamge = Sprite::createWithSpriteFrameName(str[plantsCardInformation[button->getTag()].quality - 1]);
	plantsLevelIamge->setPosition(Vec2(155, 74));
	plantsLevelIamge->setScale(0.65f);
	button->addChild(plantsLevelIamge);

	auto background = Sprite::createWithSpriteFrameName("PlantsIconBackground.png");
	background->setOpacity(0);
	button->addChild(background);

	auto plantsIconBackground = Sprite::createWithSpriteFrameName("PlantsIconBackground.png");
	plantsIconBackground->setPosition(Vec2(95, 150));
	plantsIconBackground->setScale(0.28f);
	plantsIconBackground->setColor(plantsCardInformation[button->getTag()].color);
	background->addChild(plantsIconBackground);

	auto plantsIcon = Sprite::createWithSpriteFrameName(plantsCardInformation[button->getTag()].information[1] + ".png");
	plantsIcon->setPosition(Vec2(75, 75));
	plantsIcon->setScale(1.2f);
	plantsIconBackground->addChild(plantsIcon);

	/*auto likeImage = Sprite::createWithSpriteFrameName("heart2.png");
	likeImage->setPosition(Vec2(23, 18));
	likeImage->setScale(0.45f);
	likeImage->setName("likeImage");
	likeImage->setVisible(_plantIsSelectLike[static_cast<unsigned int>(plantsCardInformation[priority].type)] &&
		isPlantIsCanSelect[static_cast<unsigned int>(plantsCardInformation[priority].type)] && showHeart);
	button->addChild(likeImage);*/

	auto prohibit = Sprite::createWithSpriteFrameName("Prohibit.png");
	prohibit->setPosition(Vec2(160, 20));
	prohibit->setScale(0.5f);
	prohibit->setVisible(!isPlantIsCanSelect[static_cast<unsigned int>(plantsCardInformation[button->getTag()].type)]);
	button->addChild(prohibit, 1);

	/* 文本 */
	auto plantsNeedSuns = ui::Text::create();
	plantsNeedSuns->setFontName(GAME_FONT_NAME_3);
	plantsNeedSuns->setColor(Color3B::BLACK);
	plantsNeedSuns->setAnchorPoint(Vec2(1, 0.5f));
	plantsNeedSuns->setPosition(Vec2(175, 18));
	plantsNeedSuns->setFontSize(35);
	plantsNeedSuns->setString(to_string(plantsCardInformation[button->getTag()].plantsNeedSunNumbers));
	button->addChild(plantsNeedSuns);

	auto plantsLevelText = ui::Text::create();
	plantsLevelText->setFontName(GAME_FONT_NAME_1);
	plantsLevelText->setColor(Color3B::BLACK);
	plantsLevelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	plantsLevelText->setPosition(Vec2(153, 75));
	plantsLevelText->setFontSize(25);
	plantsLevelText->setString(plantsCardInformation[button->getTag()].information[2]);
	button->addChild(plantsLevelText);

	return plantsNeedSuns;
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

void SPSSpriteLayer::controlPlantCanSelect(Button* button, int priority)
{
	const auto coinNumber = _global->userInformation->getCoinNumbers();
	const auto killZombiesNumber = _global->userInformation->getKillZombiesNumbers();

	if (static_cast<unsigned int>(plantsCardInformation[priority].type) >= 9)
	{
		if (coinNumber < plantsCardInformation[priority].requirement.x ||
			killZombiesNumber < plantsCardInformation[priority].requirement.y)
		{
			button->setColor(Color3B(70, 70, 70));
			button->setCascadeColorEnabled(true);  /* 设置父节点影响子节点 */
			isPlantIsCanSelect[static_cast<unsigned int>(plantsCardInformation[priority].type)] = false;
		}
		else
		{
			auto cardStr = StringUtils::format("CardUnlock_%d", static_cast<int>(plantsCardInformation[priority].type));
			if (!UserData::getInstance()->openBoolUserData(const_cast<char*>(cardStr.c_str())))
			{
				PlantsCardUnlock::unlockPlantsCard.push_back(plantsCardInformation[priority].type);
				UserData::getInstance()->caveUserData(const_cast<char*>(cardStr.c_str()), true);
			}
		}
	}
}

void SPSSpriteLayer::createPlantsText(const unsigned int& ID, const std::string& name, const Vec2& vec2, const float& fontsize, Color3B color, bool AnchorPoint)
{
	animationText[ID]->setString(name);
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
	for (unsigned int i = 0; i < 4; ++i)animationText[i]->setString("");
	if (_plantsAnimation)
	{
		_plantsAnimation->removeFromParent();
		delete _plant;
		_plant = nullptr;
	}

	_plant = GSAnimationLayer::createDifferentPlants(type, animationLayerInformation);
	_plantsAnimation = _plant->showPlantAnimationAndText();
	if (_plantsAnimation) _seedChooser->addChild(_plantsAnimation, 2);
}

string SPSSpriteLayer::selectRequirementText(map<string, LanguageTextAttribute*>& lta, PlantsType type, string str, string str1)
{
	if (isPlantIsCanSelect[static_cast<unsigned int>(type)])
	{
		return lta.find(str)->second->text;
	}
	else
	{
		return StringUtils::format(lta.find(str1)->second->text.c_str(),
			plantsCardInformation[static_cast<unsigned int>(type)].requirement.x,
			plantsCardInformation[static_cast<unsigned int>(type)].requirement.y);
	}
}

void SPSSpriteLayer::popUnlockDialog()
{
	if (PlantsCardUnlock::unlockPlantsCard.size())
	{
		pauseButtonHoverEffect();
		Director::getInstance()->getRunningScene()->addChild(PlantsCardUnlock::create(), 3);
	}
}