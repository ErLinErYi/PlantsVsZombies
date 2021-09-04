/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#include "LZAGSControlLayer.h"
#include "LZAGSDefine.h"
#include "LZAGSData.h"
#include "LZAGSGameResultJudgement.h"
#include "LZAGSGameEndLayer.h"
#include "LZAGSButtonLayer.h"
#include "LZAGSInformationLayer.h"
#include "LZAGSBackgroundLayer.h"
#include "LZAGSAnimationLayer.h"
#include "LZAGSPauseQuitLayer.h"
#include "LZAGSZombiesAppearControl.h"

#include "Plants/LZPlants.h"
#include "Plants/LZPlants-Files.h"
#include "Zombies/LZZombies.h"
#include "Based/LZLevelData.h"
#include "Based/LZGameType.h"
#include "Based/LZUserData.h"
#include "Based/LZPlayMusic.h"
#include "Scenes/EasterEggsScene/LZGameEasterEggs.h"

GameMapInformation::GameMapInformation(unsigned int row, unsigned int column):
   rowNumbers(row)
,  columnNumbers(column)
,  mapLeft(570)
,  mapRight(1690)
,  mapTop(810)
,  mapBottom(110)
{
}

void GameMapInformation::GameMapInit()
{
	MAP_INIT(plantsMap, NO_PLANTS);
	MAP_INIT(plantPumpkin, false);
	MAP_CAN_NOT_PLANT(plantsMap);
}

void GameMapInformation::GameMapInitForIZombies()
{
	MAP_INIT(plantsMap, NO_PLANTS);
}

GSControlLayer::GSControlLayer():
    gameMapInformation(nullptr)
,	_global(Global::getInstance())
,   _openLevelData(OpenLevelData::getInstance())
,   _selectPlantsTag(PlantsType::None)
,   _plantPreviewImage(nullptr)
,   _gameEndShieldLayer(nullptr)
,   _zombiesAppearControl(nullptr)
,   _listener(nullptr)
,   _cur(SET_OUT_MAP)
,   _isShowEggScene(false)
,   _isImitater(false)
{
}

GSControlLayer::~GSControlLayer()
{
	if(gameMapInformation)delete gameMapInformation;
	if(_zombiesAppearControl)delete _zombiesAppearControl;
}

bool GSControlLayer::init()
{
	if(!Layer::init())return false;

	initData();
	createSchedule();
	createPlantsCardListener();
	createMouseListener();

	return true;
}

void GSControlLayer::initData()
{
	srand(time(nullptr));
	gameMapInformation = new GameMapInformation();
	gameMapInformation->GameMapInit();
	_zombiesAppearControl = new ZombiesAppearControl();
	_levelData = _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getMunchZombiesFrequency();
	_levelZombiesFrequence = _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesFrequency();
}

void GSControlLayer::setPlantMapCanPlant(const unsigned int colum, const unsigned int row, PlantsType type)
{
	if (type != PlantsType::Imitater)
	{
		if (type != PlantsType::Pumpkin)
		{
			controlLayerInformation->gameMapInformation->plantsMap[colum][row] = NO_PLANTS;
		}
		else
		{
			controlLayerInformation->gameMapInformation->plantPumpkin[colum][row] = false;
		}
	}
}

void GSControlLayer::createSchedule()
{
	schedule([&](float){
			controlCardEnabled();
			createZombies();
			controlRefurbishMusicAndText();
		}, 0.1f, "quickControl");

	schedule([&](float) {
		judgeLevelIsFinished();
		zombiesComeTiming();
		}, 1.0f, "slowControl");
}

void GSControlLayer::controlCardEnabled()
{
	for (auto& card : _global->userInformation->getUserSelectCrads())
	{
		/* 如果植物所需阳光数大于拥有阳光数 */
		if (buttonLayerInformation->plantsCards[card.cardTag].plantsNeedSunNumbers > _global->userInformation->getSunNumbers())
		{
			buttonLayerInformation->plantsCards[card.cardTag].plantsCardText->setColor(Color3B::RED);
		}
		else
		{
			buttonLayerInformation->plantsCards[card.cardTag].plantsCardText->setColor(Color3B::WHITE);
		}
		/* 如果卡牌填充倒计时完成 */
		if (buttonLayerInformation->plantsCards[card.cardTag].timeBarIsFinished)
		{
			buttonLayerInformation->plantsCards[card.cardTag].plantsCards->setEnabled(true);
			/* 如果植物所需阳光数小于阳光数 */
			if (buttonLayerInformation->plantsCards[card.cardTag].plantsNeedSunNumbers <= _global->userInformation->getSunNumbers())
			{
				buttonLayerInformation->plantsCards[card.cardTag].plantsCards->setColor(Color3B::WHITE);
			}
			else
			{
				buttonLayerInformation->plantsCards[card.cardTag].plantsCards->setColor(Color3B::GRAY);
			}
		}
	}
}

void GSControlLayer::calculatePlantPosition()
{
	/* 如果不在范围内，移除到画面外 */
	if (GRASS_OUTSIDE(_cur))
	{
		_plantsPosition.x = 9;
		_plantsPosition.y = 5;
		return;
	}

	if ((_plantsPosition.x < 0 || _plantsPosition.x > 8 || _plantsPosition.y < 0 || _plantsPosition.y > 4) &&
		(_plantsPosition.x != 9 || _plantsPosition.y != 5))
	{
		_plantsPosition.x = 9;
		_plantsPosition.y = 5;
		return;
	}

	for (unsigned int i = 0; i < gameMapInformation->rowNumbers; ++i)
	{
		for (unsigned int j = 0; j < gameMapInformation->columnNumbers; ++j)
		{
			if (GRASS_INSIDE(_cur, i, j) && (_plantsPosition.x != j || _plantsPosition.y != i))
			{
				_plantsPosition.x = j;
				_plantsPosition.y = i;
			}
		}
	}
}

void GSControlLayer::createMouseListener()
{
	/* 创建鼠标监听 */
	_listener = EventListenerMouse::create();

	/* 鼠标移动 */
	_listener->onMouseMove = [&](Event* event)
	{
		/* 获取鼠标位置 */
		_cur = ((EventMouse*)event)->getLocationInView();
		calculatePlantPosition();
		mouseMoveControl();
		showSelectedButtonHoverEffect(_cur);
	};

	/* 鼠标按下 */
	_listener->onMouseDown = [&](Event* event)
	{
		_cur = ((EventMouse*)event)->getLocationInView();
		mouseDownControl((EventMouse*)event);
	};

	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);
}

void GSControlLayer::createPlantsCardListener()
{
	int t = -1;
	for (int i = _global->userInformation->getUserSelectCrads().size() - 1; i >= 0; --i)
	{
		if (_global->userInformation->getUserSelectCrads()[i].cardTag == static_cast<int>(PlantsType::Imitater))
		{
			t = i - 1;
			break;
		}
	}

	for (auto& card : _global->userInformation->getUserSelectCrads())
	{
		buttonLayerInformation->plantsCards[card.cardTag].plantsCards->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case ui::Widget::TouchEventType::ENDED:
					if (!buttonLayerInformation->mouseSelectImage->isSelectPlants)
					{
						_selectPlantsTag = static_cast<PlantsType>(card.cardTag);
						if (static_cast<PlantsType>(card.cardTag) == PlantsType::Imitater)
						{
							buttonLayerInformation->mouseSelectImage->selectPlantsId= static_cast<PlantsType>(_global->userInformation->getUserSelectCrads()[t].cardTag);
							_isImitater = true;
						}
						else
						{
							buttonLayerInformation->mouseSelectImage->selectPlantsId = _selectPlantsTag;
							_isImitater = false;
						}
					}
					selectPlantsPreviewImage();
					break;
				}
			});
	}
}

void GSControlLayer::showSelectedButtonHoverEffect(Vec2 vec2)
{
	for (auto& card : _global->userInformation->getUserSelectCrads())
	{
		if (!buttonLayerInformation->mouseSelectImage->isSelectPlants)
		{
			if (buttonLayerInformation->plantsCards[card.cardTag].timeBarIsFinished &&
				buttonLayerInformation->plantsCards[card.cardTag].plantsNeedSunNumbers <= _global->userInformation->getSunNumbers())
			{
				buttonLayerInformation->plantsCards[card.cardTag].plantsCards->getChildByName("seedPacketFlash")->setVisible(
					buttonLayerInformation->plantsCards[card.cardTag].plantsCards->getBoundingBox().containsPoint(vec2));
			}
			else
			{
				buttonLayerInformation->plantsCards[card.cardTag].plantsCards->getChildByName("seedPacketFlash")->setVisible(false);
			}
		}
	}
}

void GSControlLayer::selectPlantsPreviewImage()
{
	switch (buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
	case true:
		PlayMusic::playMusic("tap2");
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].progressTimer->setPercentage(0);
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->setColor(Color3B::WHITE);

		/* 加上所需的阳光数并更新 */
		_global->userInformation->setSunNumbers(_global->userInformation->getSunNumbers() + 
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsNeedSunNumbers);
		informationLayerInformation->updateSunNumbers();

		removePreviewPlant();

		/* 植物要求更新 */
		informationLayerInformation->gameType->updateRequirementNumbers("植物数量增加");
		break;
	case false:
		/* 如果植物所需阳光大于拥有阳光，则发出警告 */
		if (buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsNeedSunNumbers > _global->userInformation->getSunNumbers())
		{
			PlayMusic::playMusic("buzzer");
			informationLayerInformation->sunNumberTextWarning();
		}
		/* 剩余植物数量小于等于0 */
		else if (informationLayerInformation->gameType->getPlantsRequriement()->isHavePlantsRequriement && informationLayerInformation->gameType->getPlantsRequriement()->surPlusPlantsNumbers <= 0)
		{
			PlayMusic::playMusic("buzzer");
			informationLayerInformation->gameType->waringPlantsNull();
		}
		else
		{
			PlayMusic::playMusic("seedlift");
			
			/* 减去所需的阳光数并更新 */
			_global->userInformation->setSunNumbers(_global->userInformation->getSunNumbers() - buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsNeedSunNumbers);
			informationLayerInformation->updateSunNumbers();

			/* 卡片变黑并出现选中效果 */
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->setColor(Color3B::GRAY);
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].progressTimer->setPercentage(100);
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setVisible(true);
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setColor(Color3B::ORANGE);

			/* 鼠标选择标记 */
			buttonLayerInformation->mouseSelectImage->isSelectPlants = true;
			//buttonLayerInformation->mouseSelectImage->selectPlantsId = _selectPlantsTag;

			createPreviewPlants();

			/* 植物要求更新 */
			informationLayerInformation->gameType->updateRequirementNumbers("植物数量减少");
		}
		break;
	}
}

void GSControlLayer::createPreviewPlants()
{
	CURSOR_VISIBLE(false)

	Plants* previewPlants, * curPlants;/* 预览植物 */
	previewPlants = GSAnimationLayer::createDifferentPlants(_selectPlantsTag, animationLayerInformation);
	curPlants = GSAnimationLayer::createDifferentPlants(_selectPlantsTag, animationLayerInformation);
	
	_plantPreviewImage = previewPlants->createPlantImage();

	_plantCurImage = curPlants->createPlantImage();
	_plantCurImage->setOpacity(255);
	_plantCurImage->setPosition(_cur);
	_plantCurImage->setGlobalZOrder(10);
}

void GSControlLayer::zombiesComeTiming()
{
	if (!_zombiesAppearControl->getIsBegin())
	{
		_zombiesAppearControl->setTimeClear();
		_zombiesAppearControl->setIsBegin(true);
	}

	/* 计时 */
	_zombiesAppearControl->setTimeAdd();
}

void GSControlLayer::createZombies()
{
	/* 刷新僵尸 */
	if (_zombiesAppearControl->getLastFrequencyZombiesWasDeath())
	{
		_zombiesAppearControl->setLastFrequencyZombiesWasDeath(false);
		_zombiesAppearControl->setTimeClear(); /* 距离上一波刷新时间清零 */
		if (_zombiesAppearControl->getZombiesAppearFrequency() < _levelZombiesFrequence)
		{
			const unsigned int zombiesNumbers = _zombiesAppearControl->getZombiesNumbersForAppearFrequency(_zombiesAppearControl->getZombiesAppearFrequency());
			for (unsigned int i = 0; i < zombiesNumbers; ++i)
			{
				animationLayerInformation->createZombies();
			}
			/* 进攻波数自增一 */
			_zombiesAppearControl->setZombiesAppearFrequency();

			/* 进度条更新 */
			informationLayerInformation->updateProgressBar(_zombiesAppearControl->getZombiesAppearFrequency());
		}
	}
	informationLayerInformation->updateProgressBarHead();
	
	/* 控制僵尸的刷新 */
	if (controlRefurbishZombies())
	{
		_zombiesAppearControl->setLastFrequencyZombiesWasDeath(true);
		_zombiesAppearControl->setIsBegin(false);

		buttonLayerInformation->nextWaveButton->setVisible(false);
	}
	else
	{
		if (_zombiesAppearControl->getZombiesAppearFrequency() > 2 &&                        /* 波数大于1 */
			_zombiesAppearControl->getZombiesAppearFrequency() < _levelZombiesFrequence &&   /* 波数小于最大波数 */
			_zombiesAppearControl->getTime() > 10 &&                                         /* 时间大于10秒 */
			!buttonLayerInformation->nextWaveButton->isVisible())                            /* 按钮没有出现 */
		{
			buttonLayerInformation->nextWaveButton->setVisible(true);
		}
	}
}

bool GSControlLayer::controlRefurbishZombies()
{
	if ((Zombies::getZombiesNumbers() <= 4 &&
		_zombiesAppearControl->getZombiesAppearFrequency() > 3)                    /* 如果活着的僵尸数小于规定，刷新下一波 */

		|| (Zombies::getZombiesNumbers() <= 0 &&                                   /* 如果没有存活僵尸则立即刷新僵尸 */
			_zombiesAppearControl->getZombiesAppearFrequency() >= 1)

		|| (_zombiesAppearControl->getTime() >= 
			_openLevelData->readLevelData(
				_openLevelData->getLevelNumber())->getFirstFrequencyTime() &&
			_zombiesAppearControl->getZombiesAppearFrequency() == 0)               /* 第一波刷新控制 */

		|| (_zombiesAppearControl->getTime() >= 32 + rand() % 10 &&
			(_zombiesAppearControl->getZombiesAppearFrequency() == 1 || 
		     _zombiesAppearControl->getZombiesAppearFrequency() == 2))             /* 第二三波刷新控制 */

		|| (_zombiesAppearControl->getTime() >= 40 &&
			_zombiesAppearControl->getZombiesAppearFrequency() > 2)                /* 如果大于40秒刷新下一波 */
		)
	{
		return true;
	}
	return false;
}

void GSControlLayer::controlRefurbishMusicAndText()
{
	/* 控制大波僵尸来袭的文字与音乐 */
	const auto level = _openLevelData->readLevelData(_openLevelData->getLevelNumber());
	if (_zombiesAppearControl->getTime() >= level->getFirstFrequencyTime() && _zombiesAppearControl->getZombiesAppearFrequency() == 0)
	{
		PlayMusic::playMusic("awooga");
	}

	/* 最后一波僵尸的文字显示与旗子更新 */
	if (_zombiesAppearControl->getZombiesAppearFrequency() == level->getZombiesFrequency() && !_zombiesAppearControl->getIsShowWords())
	{
		if (informationLayerInformation->updateProgressBarFlag())
		{
			_zombiesAppearControl->setIsShowWords(true);
			
			Marigold::stopCreateCoin();/* 暂停生产金币 */
		}
	}

	/* 前几波文字显示与旗子更新 */
	for (auto data = _levelData.begin(); data != _levelData.end();)
	{
		if (_zombiesAppearControl->getZombiesAppearFrequency() == *data)
		{
			if (informationLayerInformation->updateProgressBarFlag(-1) && informationLayerInformation->updateProgressBarFlag(*data))
			{
				data = _levelData.erase(data);
			}
			else
			{
				++data;
			}
		}
		else 
		{
			++data;
		}
	}
}

void GSControlLayer::updateFlag()
{
	for (auto data : _levelData)
	{
		if (_zombiesAppearControl->getZombiesAppearFrequency() > data)
		{
			informationLayerInformation->updateProgressBarFlag(data);
		}
	}
	if (_zombiesAppearControl->getIsShowWords())
	{
		informationLayerInformation->updateProgressBarFinalFlag();
	}
}

bool GSControlLayer::judgeMousePositionIsInMap()
{
	return (_plantsPosition.x >= 0 && _plantsPosition.x < gameMapInformation->columnNumbers &&
		_plantsPosition.y >= 0 && _plantsPosition.y < gameMapInformation->rowNumbers) ? true : false;
}

bool GSControlLayer::judgeMousePositionIsCanPlant()
{
	auto n = gameMapInformation->plantsMap[static_cast<unsigned int>(_plantsPosition.y)][static_cast<unsigned int>(_plantsPosition.x)];
	auto m = gameMapInformation->plantPumpkin[static_cast<unsigned int>(_plantsPosition.y)][static_cast<unsigned int>(_plantsPosition.x)];

	if (buttonLayerInformation->mouseSelectImage->selectPlantsId == PlantsType::Pumpkin)
	{
		return (n != CAN_NOT_PLANT && m == false);
	}
	else
	{
		return (n != CAN_NOT_PLANT && n == NO_PLANTS);
	}
}

bool GSControlLayer::judgeMousePositionHavePlant()
{
	auto n = gameMapInformation->plantsMap[static_cast<unsigned int>(_plantsPosition.y)][static_cast<unsigned int>(_plantsPosition.x)];
	auto m = gameMapInformation->plantPumpkin[static_cast<unsigned int>(_plantsPosition.y)][static_cast<unsigned int>(_plantsPosition.x)];

	if (m)
	{
		return true;
	}
	else
	{
		return  (n != CAN_NOT_PLANT && n != NO_PLANTS);
	}
}

void GSControlLayer::removePreviewPlant()
{
	/* 移除预览植物 */
	_plantPreviewImage->removeFromParent();
	_plantCurImage->removeFromParent();
	buttonLayerInformation->mouseSelectImage->isSelectPlants = false;
	CURSOR_VISIBLE(true)
}

void GSControlLayer::removeShovel()
{
	buttonLayerInformation->mouseSelectImage->isSelectShovel = false;
	buttonLayerInformation->updateShovel();
	buttonLayerInformation->shovelImage->removeFromParent();
}

void GSControlLayer::removeMouseListener()
{
	if (_listener)
	{
		_director->getEventDispatcher()->removeEventListener(_listener);
	}
}

void GSControlLayer::recoveryPlantsColor()
{
	for (unsigned int i = 0; i < gameMapInformation->rowNumbers; ++i)
	{
		for (unsigned int j = 0; j < gameMapInformation->columnNumbers; ++j)
		{
			if (gameMapInformation->plantsMap[i][j] != CAN_NOT_PLANT && gameMapInformation->plantsMap[i][j] != NO_PLANTS)
			{
				auto plant = animationLayerInformation->getChildByTag(SET_TAG(Vec2(j, i)));
				if (plant)
				{
					plant->setColor(Color3B::WHITE);
				}
			}
			if (gameMapInformation->plantsMap[i][j] != CAN_NOT_PLANT && gameMapInformation->plantPumpkin[i][j])
			{
				auto plant = animationLayerInformation->getChildByTag(SET_TAG(Vec2(j, i)) + Pumpkin::tagAddition);
				if (plant)
				{
					plant->setColor(Color3B::WHITE);
				}
			}
		}
	}
}

void GSControlLayer::judgeLevelIsFinished()
{
	/* 关卡结束 */
	if (ZombiesGroup.size() <= 0 && _zombiesAppearControl->getZombiesAppearFrequency() >=
		_openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesFrequency())
	{
		CURSOR_VISIBLE(true)

		setGameEnd();

		auto judgeUserWin = new GSGameResultJudgement();
		const auto winOrLose = judgeUserWin->judgeUserIsWin();
		if (winOrLose == GameTypes::None)
		{
			if (_global->userInformation->getCurrentPlayLevels() >= 52 && !_isShowEggScene)
			{
				_isShowEggScene = true;
				_director->getInstance()->pushScene(TransitionFade::create(0.5f, GameEasterEggs::create()));
			}
			else
			{
				_gameEndShieldLayer->successfullEntry();
			}
		}
		else
		{
			_gameEndShieldLayer->breakThrough(winOrLose); /* 闯关失败 */
		}
		delete judgeUserWin;
	}
}

void GSControlLayer::setGameEnd()
{
	_gameEndShieldLayer = GSGameEndLayer::create();
	_director->getRunningScene()->addChild(_gameEndShieldLayer, 10, "gameEndShieldLayer");
	_director->getScheduler()->setTimeScale(1.0f); /* 恢复播放倍数 */

	GSPauseQuitLayer::pauseLayer();
}

void GSControlLayer::mouseMoveControl()
{
	/* 如果鼠标选择了植物 */
	if (buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
		const int posX = static_cast<int>(_plantsPosition.x);
		const int posY = static_cast<int>(_plantsPosition.y);
		if (posX >= 0 && posY >= 0 && posX < 9 && posY < 5)
		{
			if (buttonLayerInformation->mouseSelectImage->selectPlantsId == PlantsType::Pumpkin)
			{
				if (gameMapInformation->plantPumpkin[posY][posX] == false)
				{
					const auto size = _plantPreviewImage->getContentSize() / 2.f;
					_plantPreviewImage->setPosition(Vec2(GRASS_POSITION_LEFT + 122 *
						_plantsPosition.x + size.width, GRASS_POSITION_BOTTOM + 138 * (_plantsPosition.y + 1) - size.height));
				}
				else
				{
					_plantPreviewImage->setPosition(SET_OUT_MAP);
				}
			}
			else
			{
				if (gameMapInformation->plantsMap[posY][posX] != NO_PLANTS)
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
		}
		else
		{
			_plantPreviewImage->setPosition(SET_OUT_MAP);
		}
		_plantCurImage->setPosition(_cur + Vec2(0, 30));
	}

	/* 鼠标上有铲子 */
	if (buttonLayerInformation->mouseSelectImage->isSelectShovel)
	{
		/* 循环把植物恢复到原来的颜色 */
		recoveryPlantsColor();

		if (judgeMousePositionIsInMap() && judgeMousePositionHavePlant())  /* 如果在地图范围内 && 种有植物 */
		{
			checkPlantType(1);
		}
		buttonLayerInformation->shovelImage->setPosition(_cur + Vec2(35, 50));
	}
}

void GSControlLayer::mouseDownControl(EventMouse* eventmouse)
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

void GSControlLayer::mouseLeftButtonDownControl()
{
	if (buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
		if (judgeMousePositionIsInMap() && judgeMousePositionIsCanPlant() && _cur.x > CARD_BAR_RIGHT) /* 如果在地图范围内 && 可以种植植物 */
		{
			/* 记录使用植物数量 */
			UserData::getInstance()->caveUserData(const_cast<char*>("USEPLANTSNUMBERS"), ++_global->userInformation->getUsePlantsNumbers());

			if (buttonLayerInformation->mouseSelectImage->selectPlantsId == PlantsType::Pumpkin)
			{
				gameMapInformation->plantPumpkin[static_cast<unsigned int>(_plantsPosition.y)][static_cast<unsigned int>(_plantsPosition.x)] = true;
			}
			else
			{
				/* 地图记录种植的植物 */
				gameMapInformation->plantsMap[static_cast<unsigned int>(_plantsPosition.y)][static_cast<unsigned int>(_plantsPosition.x)] =
					static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId);
			}

			if (_isImitater)
			{
				buttonLayerInformation->mouseSelectImage->selectPlantsId = PlantsType::Imitater;
			}
			/* 种植植物 */
			animationLayerInformation->plantPlants();

			/* 设置倒计时并且按钮不可用 */
			const unsigned int plantsTag = static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId);
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].timeBarIsFinished = false;
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCards->setEnabled(false);
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCards->setColor(Color3B::GRAY);
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].progressTimer->runAction(
				Sequence::create(ProgressFromTo::create(buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCoolTime, 100, 0),
					CallFunc::create([=]() { buttonLayerInformation->plantsCards[plantsTag].timeBarIsFinished = true; }), nullptr)
			);
			removePreviewPlant();

			buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setColor(Color3B::WHITE);
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setVisible(false);
		}
		else
		{
			if (_cur.x > CARD_BAR_RIGHT)
			{
				PlayMusic::playMusic("buzzer");

				/* 提示信息 */
				informationLayerInformation->createPromptText();
			}
			else
			{
				buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setColor(Color3B::WHITE);
				buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setVisible(false);
			}
		}
	}

	if (buttonLayerInformation->mouseSelectImage->isSelectShovel) /* 鼠标上有铲子 */
	{
		if (judgeMousePositionIsInMap() && judgeMousePositionHavePlant())    /* 如果在地图范围内 && 种有植物 */
		{
			PlayMusic::playMusic("plant2");
			checkPlantType();
			removeShovel();
		}
		else
		{
			if (!buttonLayerInformation->getChildByName("ShovelBank")->getBoundingBox().containsPoint(_cur))
			{
				removeShovel();
			}
			PlayMusic::playMusic("shovel");
		}
		recoveryPlantsColor();
	}
}

void GSControlLayer::mouseRightButtonDownControl()
{
	if (buttonLayerInformation->mouseSelectImage->isSelectPlants)/* 鼠标上有植物 */
	{
		if (_cur.x > CARD_BAR_RIGHT)
		{
			if (_isImitater)
			{
				buttonLayerInformation->mouseSelectImage->selectPlantsId = PlantsType::Imitater;
			}

			PlayMusic::playMusic("tap2");
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].progressTimer->setPercentage(0);
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCards->setColor(Color3B::WHITE);

			/* 加上所需的阳光数并更新 */
			_global->userInformation->setSunNumbers(_global->userInformation->getSunNumbers() +
				buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsNeedSunNumbers);
			informationLayerInformation->updateSunNumbers();

			/* 植物要求更新 */
			informationLayerInformation->gameType->updateRequirementNumbers("植物数量增加");

			removePreviewPlant();
		}

		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setColor(Color3B::WHITE);
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setVisible(false);
	}

	if (buttonLayerInformation->mouseSelectImage->isSelectShovel) /* 鼠标上有铲子 */
	{
		removeShovel();
		recoveryPlantsColor();
	}
}

void GSControlLayer::mouseMiddleButtonDownControl()
{
}

void GSControlLayer::checkPlantType(const int type)
{
	auto n = gameMapInformation->plantsMap[static_cast<unsigned int>(_plantsPosition.y)][static_cast<unsigned int>(_plantsPosition.x)];
	auto m = gameMapInformation->plantPumpkin[static_cast<unsigned int>(_plantsPosition.y)][static_cast<unsigned int>(_plantsPosition.x)];

	if (n != CAN_NOT_PLANT && (n != NO_PLANTS || m))
	{
		auto plant = animationLayerInformation->getChildByTag(SET_TAG(_plantsPosition));
		auto plant1 = animationLayerInformation->getChildByTag(SET_TAG(_plantsPosition) + Pumpkin::tagAddition);

		if (type == 0)
		{
			if (plant && plant1 && plant->isVisible() && plant1->isVisible())
			{
				if (plant1->getBoundingBox().containsPoint(_cur))
				{
					animationLayerInformation->deletePlants(1);/* 铲除植物 */
				}
				else if(plant->getBoundingBox().containsPoint(_cur))
				{
					animationLayerInformation->deletePlants();/* 铲除植物 */
				}
			}
			else
			{
				if (plant)animationLayerInformation->deletePlants();
				if (plant1)animationLayerInformation->deletePlants(1);
			}
		}
		else
		{
			if (plant && plant1 && plant->isVisible() && plant1->isVisible())
			{
				if (plant1->getBoundingBox().containsPoint(_cur))
				{
					plant1->setColor(Color3B(100, 100, 100));
				}
				else if (plant->getBoundingBox().containsPoint(_cur))
				{
					plant->setColor(Color3B(100, 100, 100));
				}
			}
			else
			{
				if (plant)plant->setColor(Color3B(100, 100, 100));
				if (plant1)plant1->setColor(Color3B(100, 100, 100));
			}
		}
	}
}
