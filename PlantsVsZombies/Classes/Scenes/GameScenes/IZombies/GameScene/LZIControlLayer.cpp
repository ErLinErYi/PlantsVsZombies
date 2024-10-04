/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.02
 *Email: 2117610943@qq.com
 */

#include "LZIControlLayer.h"
#include "LZIAnimationLayer.h"
#include "LZIGameEndLayer.h"
#include "LZIOpenCaveGameLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSInformationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/HammerZombies/GameScene/LZHGameEndLayer.h"
#include "Zombies/LZZombies.h"
#include "Plants/DefensePlants/LZSunFlower.h"
#include "plants/BattlePlants/LZPotatoMine.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZUserData.h"
#include "Based/LZMouseEventControl.h"
#include "Based/LZCoin.h"

unsigned int IControlLayer::_encryptMKey = 1;
unsigned int IControlLayer::_encryptCKey = 1;
unsigned int IControlLayer::currentLevelNumber = 1;
unsigned int IControlLayer::mostLevelNumber = 1;

IControlLayer::IControlLayer():
	_sunFlowerNumbers(0),
	_totalNumber(25),
	_pultPlantNumbers(0),
	_pultPlantRange(2),
	_otherPlantRange(14),
	_levelFinished(false)
{
	_random.seed(_device());
	_selectZombiesCard.button = nullptr;
}

IControlLayer::~IControlLayer()
{
}

bool IControlLayer::init()
{
	if (!Layer::init())return false;

	initData();
	createSchedule();
	createZombiesCardListener();
	createMouseListener();
	
	return true;
}

void IControlLayer::initData()
{
	gameMapInformation = new GameMapInformation();
	gameMapInformation->GameMapInitForIZombies();
	MAP_INIT(gameMapInformation->plantsMap, NO_PLANTS);
	MAP_INIT(gameMapInformation->plantPumpkin, false);

	if (!IOpenCaveGameLayer::isReadFile)
	{
		showBlackFadeOutAnimation();

		_global->userInformation->setSunNumbers(onGetCurrentLevel() > 1 ? UserData::getInstance()->openIntUserData(const_cast<char*>("IZOMBIES_SUN_NUMBER")) : 400);
		informationLayerInformation->updateSunNumbers();

		calculatePlantsNumbers();
		selectPlantsType();
	}
}

void IControlLayer::createSchedule()
{
	schedule([&](float) {
		controlCardEnabled();
		}, 0.1f, "quickControl");

	schedule([&](float) {
		judgeLevelIsFinished();
		judgeZombiesFadeOut();
		judgeUserLose();
		}, 1.0f, "slowControl");
}

void IControlLayer::controlCardEnabled()
{
	for (auto& card : IButtonLayer::zombiesCardButton)
	{
		/* 如果植物所需阳光数大于拥有阳光数 */
		if (static_cast<int>(card.needSunNumber) > _global->userInformation->getSunNumbers() ||
			(buttonLayerInformation->mouseSelectImage->isSelectPlants && _selectZombiesCard.type == card.type))
		{
			card.button->setColor(Color3B(80, 80, 80));
		}
		else
		{
			card.button->setColor(Color3B::WHITE);
		}
	}
}

void IControlLayer::createZombiesCardListener()
{
	for (auto& card : IButtonLayer::zombiesCardButton)
	{
		card.button->addTouchEventListener([&, card](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case cocos2d::ui::Widget::TouchEventType::ENDED:
					if (!buttonLayerInformation->mouseSelectImage->isSelectPlants)
					{
						_selectZombiesCard = card;
					}
					selectZombiesPreviewImage();
					break;
				}
			});
	}
}

void IControlLayer::selectZombiesPreviewImage()
{
	switch (buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
	case true:
		PlayMusic::playMusic("tap2");

		_selectZombiesCard.button->setColor(Color3B::WHITE);
		_selectZombiesCard.button->getChildByName("seedPacketFlash")->setColor(Color3B::WHITE);
		_selectZombiesCard.button->getChildByName("seedPacketFlash")->setVisible(false);

		/* 加上所需的阳光数并更新 */
		_global->userInformation->setSunNumbers(_global->userInformation->getSunNumbers() + _selectZombiesCard.needSunNumber);
		informationLayerInformation->updateSunNumbers();

		removePreviewPlant();
		break;
	case false:
		/* 如果僵尸所需阳光大于拥有阳光，则发出警告 */
		if (static_cast<int>(_selectZombiesCard.needSunNumber) > _global->userInformation->getSunNumbers())
		{
			PlayMusic::playMusic("buzzer");
			informationLayerInformation->sunNumberTextWarning();
		}
		else
		{
			PlayMusic::playMusic("seedlift");

			/* 减去所需的阳光数并更新 */
			_global->userInformation->setSunNumbers(_global->userInformation->getSunNumbers() - _selectZombiesCard.needSunNumber);
			informationLayerInformation->updateSunNumbers();

			/* 卡片变黑并出现选中效果 */
			_selectZombiesCard.button->setColor(Color3B(80, 80, 80));
			_selectZombiesCard.button->getChildByName("seedPacketFlash")->setVisible(true);
			_selectZombiesCard.button->getChildByName("seedPacketFlash")->setColor(Color3B::ORANGE);

			/* 鼠标选择标记 */
			buttonLayerInformation->mouseSelectImage->isSelectPlants = true;
			
			createPreviewZombie();
		}
		break;
	}
}

void IControlLayer::createPreviewZombie()
{
	CURSOR_VISIBLE(false)

	Zombies* previewZombies, * curZombies;/* 预览僵尸 */
	previewZombies = GSAnimationLayer::createDifferentZombies(_selectZombiesCard.type, animationLayerInformation);
	curZombies = GSAnimationLayer::createDifferentZombies(_selectZombiesCard.type, animationLayerInformation);

	_plantPreviewImage = previewZombies->createPreviewZombieImage();

	_plantCurImage = curZombies->createPreviewZombieImage();
	_plantCurImage->setOpacity(255);
	_plantCurImage->setPosition(_cur);
	_plantCurImage->setGlobalZOrder(10);
}

void IControlLayer::createMouseListener()
{
	/* 创建鼠标监听 */
	auto listener = EventListenerMouse::create();

	/* 鼠标移动 */
	listener->onMouseMove = [&](Event* event)
	{
		/* 获取鼠标位置 */
		_cur = ((EventMouse*)event)->getLocationInView();

		mouseMoveControl();
		calculatePlantPosition();
		showSelectedButtonHoverEffect();
	};

	/* 鼠标按下 */
	listener->onMouseDown = [=](Event* event)
	{
		_cur = ((EventMouse*)event)->getLocationInView();

		mouseDownControl((EventMouse*)event);
	};

	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void IControlLayer::mouseMoveControl()
{
	/* 如果鼠标选择了僵尸 */
	if (buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
		const int posX = static_cast<int>(_plantsPosition.x);
		const int posY = static_cast<int>(_plantsPosition.y);
		if (posX >= 0 && posY >= 0 && posX < 9 && posY < 5)
		{
			if (posX < 5)
			{
				_plantPreviewImage->setPosition(SET_OUT_MAP);
			}
			else
			{
				const auto size = _plantPreviewImage->getContentSize() / 2.f;
				_plantPreviewImage->setPosition(Vec2(GRASS_POSITION_LEFT + 122 *
					_plantsPosition.x + size.width, GRASS_POSITION_BOTTOM + 138 * (_plantsPosition.y + 1) - size.height));
			}
		}
		else
		{
			_plantPreviewImage->setPosition(SET_OUT_MAP);
		}
		_plantCurImage->setPosition(_cur + Vec2(0, 30));
	}
}

void IControlLayer::mouseDownControl(EventMouse* eventmouse)
{
	switch (eventmouse->getMouseButton())
	{
	case EventMouse::MouseButton::BUTTON_RIGHT:
		mouseRightButtonDownControl();
		break;
	case EventMouse::MouseButton::BUTTON_LEFT:
		mouseLeftButtonDownControl();
		break;
	case EventMouse::MouseButton::BUTTON_MIDDLE:
		mouseMiddleButtonDownControl();
		break;
	default:
		break;
	}
}

void IControlLayer::mouseRightButtonDownControl()
{
	if (buttonLayerInformation->mouseSelectImage->isSelectPlants)/* 鼠标上有僵尸 */
	{
		if (_cur.x > CARD_BAR_RIGHT)
		{
			PlayMusic::playMusic("tap2");
			_selectZombiesCard.button->setColor(Color3B::WHITE);
			_selectZombiesCard.button->getChildByName("seedPacketFlash")->setColor(Color3B::WHITE);
			_selectZombiesCard.button->getChildByName("seedPacketFlash")->setVisible(false);
			_selectZombiesCard.type = ZombiesType::None;

			/* 加上所需的阳光数并更新 */
			_global->userInformation->setSunNumbers(_global->userInformation->getSunNumbers() + _selectZombiesCard.needSunNumber);
			informationLayerInformation->updateSunNumbers();

			removePreviewPlant();
		}
	}
}

void IControlLayer::mouseLeftButtonDownControl()
{
	if (buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
		if (judgeMousePositionIsInMap()) /* 可以放置僵尸 */
		{
			dynamic_cast<IAnimationLayer*>(animationLayerInformation)->createZombies(_selectZombiesCard.type,_cur.x);

			_selectZombiesCard.button->setColor(Color3B::WHITE);
			_selectZombiesCard.button->getChildByName("seedPacketFlash")->setColor(Color3B::WHITE);
			_selectZombiesCard.button->getChildByName("seedPacketFlash")->setVisible(false);
			_selectZombiesCard.type = ZombiesType::None;

			removePreviewPlant();
		}
		else
		{
			PlayMusic::playMusic("buzzer");
		}
	}
}

bool IControlLayer::judgeMousePositionIsInMap()
{
	return (_plantsPosition.x >= 5 && _plantsPosition.x < gameMapInformation->columnNumbers&&
		_plantsPosition.y >= 0 && _plantsPosition.y < gameMapInformation->rowNumbers);
}

void IControlLayer::showSelectedButtonHoverEffect()
{
	for (auto button : IButtonLayer::zombiesCardButton)
	{
		if (_selectZombiesCard.type != button.type && static_cast<int>(button.needSunNumber) <= _global->userInformation->getSunNumbers())
		{
			button.button->getChildByName("seedPacketFlash")->setVisible(button.button->getBoundingBox().containsPoint(_cur));
		}
	}
}

void IControlLayer::createZombies()
{
	ZombiesGroup.sort(Zombies::compare);
}

void IControlLayer::judgeLevelIsFinished()
{
	/* 关卡结束 */
	if (IAnimationLayer::BrainGroup.empty())
	{
		bool finsihed = true;
		for (auto coin : CoinsGroup)
		{
			if (coin->getEnable())
			{
				finsihed = false;
				break;
			}
		}

		if (finsihed && !_levelFinished)
		{
			_levelFinished = true;
			runAction(Sequence::create(DelayTime::create(1.5f),
				CallFunc::create([=]()
					{
						auto gameEndShieldLayer = IGameEndLayer::create();
						_director->getRunningScene()->addChild(gameEndShieldLayer, 10, "gameEndShieldLayer");
						gameEndShieldLayer->successfullEntry();
					}), nullptr));
		}
	}
}

void IControlLayer::judgeUserLose()
{
	bool flag = false;
	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieAnimation()->isVisible())
		{
			flag = true;
			break;
		}
	}

	if (_global->userInformation->getSunNumbers() < 50 && !flag && !buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
		auto gameEndShieldLayer = IGameEndLayer::create();
		_director->getRunningScene()->addChild(gameEndShieldLayer, 10, "gameEndShieldLayer");
		gameEndShieldLayer->breakThrough();
	}
}

void IControlLayer::judgeZombiesFadeOut()
{
	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsSurvive() && zombie->getZombiePosition().x < 450)
		{
			zombie->getZombieAnimation()->runAction(Sequence::create(FadeOut::create(0.5f),
				CallFunc::create([=]() 
					{
						zombie->getZombieAnimation()->setVisible(false);
					}), nullptr));
		}

		for (auto brain =IAnimationLayer::BrainGroup.begin();brain!= IAnimationLayer::BrainGroup.end();)
		{
			if (brain->enable && zombie->getZombiePositionX() - 40 < brain->brain->getPositionX() &&
				zombie->getZombieInRow() == brain->row && zombie->getZombieIsSurvive())
			{
				PlayMusic::playMusic("moneyfalls");

				for (int i = 0; i < 10; ++i)
				{
					coinRecovery(brain->brain->getPosition() - Vec2(0, 80), i);
				}

				brain->enable = false;
				brain->brain->removeFromParent();
				IAnimationLayer::BrainGroup.erase(brain++);
			}
			else
			{
				brain++;
			}
		}
	}
}

void IControlLayer::calculatePlantsNumbers()
{
	// 控制向日葵数量
	if (onGetCurrentLevel() < 5)
	{
		_sunFlowerNumbers = 6 + rand() % 4;
	}
	else if (onGetCurrentLevel() < 10)
	{
		_sunFlowerNumbers = 4 + rand() % 4;
	}
	else
	{
		if (onGetCurrentLevel() % 5 == 0)
		{
			_sunFlowerNumbers = 8 + rand() % 4;
		}
		else
		{
			_sunFlowerNumbers = 2 + rand() % 3;
		}

	}

	// 控制投手植物数量
	_pultPlantNumbers = max(6 + rand() % 4 - static_cast<int>(onGetCurrentLevel()) / 2, 2);

	// 其他植物数量
	_totalNumber -= _sunFlowerNumbers;
	_totalNumber -= _pultPlantNumbers;

	// 控制所选植物范围
	_pultPlantRange = min(static_cast<int>(onGetCurrentLevel() / 40), 2);
	if (onGetCurrentLevel() % 5 == 0)
	{
		_otherPlantRange = min(static_cast<int>(onGetCurrentLevel() / 4) + 7, 13);
	}
	else
	{
		_otherPlantRange = min(static_cast<int>(onGetCurrentLevel() / 4) + 7, 13) + min(max(0, static_cast<int>(onGetCurrentLevel()) - 24) / 20, 4);
	}
}

void IControlLayer::selectPlantsType()
{
	PlayMusic::playMusic("finalwave");

	// 必出植物 向日葵、投手植物
	PlantsType sunFlowerType[] = { PlantsType::SunFlower };
	uniform_int_distribution<int>sunFlowerTypeRandom(0, 0);
	createSelectPlantsType(sunFlowerType, _sunFlowerNumbers, sunFlowerTypeRandom);

	// 投手植物 卷心菜投手-西瓜投手-冰瓜投手
	PlantsType pultType[] = { PlantsType::CabbagePult,PlantsType::WaterMelonPult,PlantsType::WinterMelonPult };
	uniform_int_distribution<int>pultTypeRandom(0, _pultPlantRange);
	createSelectPlantsType(pultType, _pultPlantNumbers, pultTypeRandom);

	// 其他随机植物
	PlantsType otherPlantType[] =
	{
		PlantsType::PeaShooter,PlantsType::WallNut,PlantsType::PotatoMine,
		PlantsType::Torchwood,PlantsType::Spikeweed,PlantsType::ScaredyShroom,
		PlantsType::IceBergLettuce,PlantsType::Chomper,PlantsType::Squash,
		PlantsType::IcePeaShooter,PlantsType::AcidLemonShooter,PlantsType::MagnetShroom,
		PlantsType::ThreePeaShooter,PlantsType::StarFruit,PlantsType::FumeShroom,
		PlantsType::Citron,PlantsType::CatTail,PlantsType::GloomShroom
	};
	uniform_int_distribution<int>otherPlantTypeRandom(0, _otherPlantRange);
	createSelectPlantsType(otherPlantType, _totalNumber, otherPlantTypeRandom);
}

void IControlLayer::createSelectPlantsType(PlantsType *type, int plantsNumber, uniform_int_distribution<int> &rand_engine)
{
	int strongPlant = 5;

	while (plantsNumber > 0)
	{
		int i = 0, j = 0;
		do
		{
			uniform_int_distribution<int>number(0, 4);
			i = number(_random);
			j = number(_random);
		} while (gameMapInformation->plantsMap[i][j] != NO_PLANTS);

		PlantsType ptype;
		do
		{
			ptype = type[rand_engine(_random)];
		} while (strongPlant <= 0 && (ptype == PlantsType::Citron || ptype == PlantsType::CatTail || ptype == PlantsType::GloomShroom));

		if (ptype == PlantsType::Citron || ptype == PlantsType::CatTail || ptype == PlantsType::GloomShroom)
		{
			--strongPlant;
		}

		autoCreatePlants(ptype, Vec2(i, j));
		--plantsNumber;
	}
}

void IControlLayer::autoCreatePlants(PlantsType type, Vec2 vec2)
{
	auto plants = GSAnimationLayer::createDifferentPlants(type, animationLayerInformation);
	plants->setPlantPosition(ROW_COLUMN_TO_POSITION(vec2));
	plants->setPlantLocalZOrder(SET_ANIMATION_Z_ORDER(vec2));
	plants->setPlantRowAndColumn(vec2);
	plants->setPlantTag(SET_TAG(vec2));

	if (type == PlantsType::SunFlower)
	{
		dynamic_cast<SunFlower*>(plants)->setIZombiesType(true);
	}
	if (type == PlantsType::PotatoMine)
	{
		dynamic_cast<PotatoMine*>(plants)->setBreakGround(0.01f);
	}
	plants->createPlantAnimation();

	PlantsGroup.insert(pair<int, Plants*>(plants->getPlantAnimation()->getTag(), plants));
	gameMapInformation->plantsMap[static_cast<unsigned int>(vec2.x)][static_cast<unsigned int>(vec2.y)] = static_cast<int>(type);
}

void IControlLayer::showBlackFadeOutAnimation()
{
	auto layer = LayerColor::create(Color4B::BLACK);
	layer->runAction(Sequence::create(FadeOut::create(1.f), CallFunc::create([=]() {layer->removeFromParent(); }), nullptr));
	layer->setGlobalZOrder(10);
	this->addChild(layer);
}

void IControlLayer::coinRecovery(const Vec2& position, const int id)
{
	auto coin = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("coin")->second);
	coin->setPosition(position);
	coin->update(0);
	coin->setScale(0.7f);
	coin->setOpacity(0);
	coin->setGlobalZOrder(3);
	this->addChild(coin);

	auto callFunc = CallFunc::create([=]()
		{
			PlayMusic::playMusic("moneyfalls");
			coin->removeFromParent();
			_global->userInformation->setCoinNumbers(_global->userInformation->getCoinNumbers() + 1);
			informationLayerInformation->updateCoinNumbers();
		});

	auto action = Spawn::create(FadeIn::create(0.5f), JumpBy::create(0.5f, Vec2(150 - rand() % 300, 100 - rand() % 100), rand() % 100 + 200, 1),
		Sequence::create(DelayTime::create(0.25f), nullptr), nullptr);
	auto action1 = Spawn::create(ScaleTo::create(0.2f, 0.5f), FadeOut::create(0.2f), callFunc, nullptr);

	coin->runAction(Sequence::create(DelayTime::create(id * 0.05f), action, DelayTime::create(0.05f), MoveTo::create(0.9f, Vec2(1655, 38)), action1, nullptr));
}

void IControlLayer::beginNewGame()
{
	UserData::getInstance()->caveUserData(const_cast<char*>("IZOMBIES_MOST_LEVEL"), static_cast<int>(IControlLayer::onGetMostLevel()));
	UserData::getInstance()->caveUserData(const_cast<char*>("IZOMBIES_LEVEL_NUMBER"), 1);
	UserData::getInstance()->caveUserData(const_cast<char*>("IZOMBIES_SUN_NUMBER"), 0);
}

void IControlLayer::onSetMostLevel(unsigned int level)
{
	mostLevelNumber = level;

	_encryptMKey = rand();
	mostLevelNumber ^= _encryptMKey;
}

unsigned int IControlLayer::onGetMostLevel()
{
	return mostLevelNumber ^ _encryptMKey;
}

void IControlLayer::onSetCurrentLevel(unsigned int level)
{
	currentLevelNumber = level;

	_encryptCKey = rand();
	currentLevelNumber ^= _encryptCKey;
}

unsigned int IControlLayer::onGetCurrentLevel()
{
	return currentLevelNumber ^ _encryptCKey;
}
