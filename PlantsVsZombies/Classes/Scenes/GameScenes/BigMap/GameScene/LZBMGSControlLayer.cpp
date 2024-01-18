/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.14
 *Email: 2117610943@qq.com
 */

#include "LZBMGSControlLayer.h"
#include "LZBigMapGameScene.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSZombiesAppearControl.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSButtonLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSInformationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSGameEndLayer.h"
#include "Scenes/EasterEggsScene/LZGameEasterEggs.h"
#include "Based/LZLevelData.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZUserData.h"
#include "Plants/LZPlants.h"
#include "Plants/DefensePlants/LZPumpkin.h"
#include "Zombies/LZZombies.h"

bool BMControlLayer::isLose = false;

BMControlLayer::BMControlLayer():
	_isFade(false)
{
}

BMControlLayer::~BMControlLayer()
{
	isLose = false;
}

bool BMControlLayer::init()
{
	if (!Layer::init())return false;

	initData();
	createSchedule();
	createPlantsCardListener();
	createMouseListener();

	return true;
}

void BMControlLayer::initData()
{
	srand(time(nullptr));
	gameMapInformation = new GameMapInformation(10, 18);
	MAP_INIT(gameMapInformation->plantsMap,NO_PLANTS);
	MAP_INIT(gameMapInformation->plantPumpkin, false);
	gameMapInformation->mapLeft = GRASS_BIGMAP_POSITION_LEFT;
	gameMapInformation->mapRight = GRASS_BIGMAP_POSITION_RIGHT;
	gameMapInformation->mapTop = GRASS_BIGMAP_POSITION_TOP;
	gameMapInformation->mapBottom = GRASS_BIGMAP_POSITION_BOTTOM;

	_zombiesAppearControl = new ZombiesAppearControl();
	_levelData = _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getMunchZombiesFrequency();
	_levelZombiesFrequence = _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesFrequency();
}

void BMControlLayer::createSchedule()
{
	schedule([&](float) {
		controlCardEnabled();
		createZombies();
		controlRefurbishMusicAndText();
		}, 0.1f, "mainUpdate");

	schedule([&](float) {
		zombiesComeTiming();
		judgeLevelIsFinished();
		}, 1.0f, "zombiesComing");
}

void BMControlLayer::calculatePlantPosition()
{
	/* 如果不在范围内，移除到画面外 */
	if (GRASS_BIGMAP_OUTSIDE(_cur))
	{
		_plantsPosition.x = 18;
		_plantsPosition.y = 10;
		return;
	}

	if ((_plantsPosition.x < 0 || _plantsPosition.x > 17 || _plantsPosition.y < 0 || _plantsPosition.y > 9) &&
		(_plantsPosition.x != 18 || _plantsPosition.y != 10))
	{
		_plantsPosition.x = 18;
		_plantsPosition.y = 10;
		return;
	}

	for (unsigned int i = 0; i < gameMapInformation->rowNumbers; ++i)
	{
		for (unsigned int j = 0; j < gameMapInformation->columnNumbers; ++j)
		{
			
			if (GRASS_BIGMAP_INSIDE(_cur, i, j) &&
				(_plantsPosition.x != j || _plantsPosition.y != i))
			{
				_plantsPosition.x = j;
				_plantsPosition.y = i;
			}
		}
	}
}

void BMControlLayer::createPreviewPlants()
{
	CURSOR_VISIBLE(false)

	Plants* previewPlants, * curPlants;/* 预览植物 */
	previewPlants = GSAnimationLayer::createDifferentPlants(_selectPlantsTag, animationLayerInformation);
	curPlants = GSAnimationLayer::createDifferentPlants(_selectPlantsTag, animationLayerInformation);

	_plantPreviewImage = previewPlants->createPlantImage();
	
	_plantCurImage = curPlants->createPlantImage();
	_plantCurImage->setOpacity(255);
	_plantCurImage->setPosition(SET_OUT_MAP);
	_plantCurImage->setGlobalZOrder(10);
}

void BMControlLayer::mouseDownControl(EventMouse* eventmouse)
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
		break;
	default:
		break;
	}
}

void BMControlLayer::mouseMoveControl()
{
	/* 如果鼠标选择了植物 */
	if (buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
		const int posX = static_cast<int>(_plantsPosition.x);
		const int posY = static_cast<int>(_plantsPosition.y);
		if (posX >= 0 && posY >= 0 && posX < 18 && posY < 10)
		{
			if (buttonLayerInformation->mouseSelectImage->selectPlantsId == PlantsType::Pumpkin)
			{
				if (gameMapInformation->plantPumpkin[posY][posX] == false)
				{
					const auto size = _plantPreviewImage->getContentSize() / 2.f;
					_plantPreviewImage->setPosition(Vec2(GRASS_BIGMAP_POSITION_LEFT + 77 * _plantsPosition.x + size.width,
						GRASS_BIGMAP_POSITION_BOTTOM + 86 * (_plantsPosition.y + 1) - size.height) + Vec2(-10, 60));
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
					_plantPreviewImage->setPosition(Vec2(GRASS_BIGMAP_POSITION_LEFT + 77 * _plantsPosition.x + size.width,
						GRASS_BIGMAP_POSITION_BOTTOM + 86 * (_plantsPosition.y + 1) - size.height) + Vec2(-10, 60));
				}
			}
		}
		else
		{
			_plantPreviewImage->setPosition(SET_OUT_MAP);
		}
		_plantCurImage->setPosition(_cur + Vec2(0, 50));
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
		buttonLayerInformation->shovelImage->setPosition(_cur / 2.f + Vec2(35, 50));
	}
}

void BMControlLayer::judgeLevelIsFinished()
{
	/* 关卡结束 */
	if (ZombiesGroup.size() <= 0 && _zombiesAppearControl->getZombiesAppearFrequency() >=
		_openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesFrequency())
	{
		CURSOR_VISIBLE(true)

		setGameEnd();
		_gameEndShieldLayer->successfullEntry();
	}
}

void BMControlLayer::judgeZombiesWin(Zombies* zombie)
{
	if (!isLose && zombie->getZombieAnimation()->getPositionX() < GRASS_BIGMAP_POSITION_LEFT - 120)
	{
		isLose = true;
		auto gameEndShieldLayer = GSGameEndLayer::create();
		Director::getInstance()->getRunningScene()->addChild(gameEndShieldLayer, 10, "gameEndShieldLayer");
		gameEndShieldLayer->breakThrough(GameTypes::UserLose);
	}
}

void BMControlLayer::checkPlantType(const int type)
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
				else if (plant->getBoundingBox().containsPoint(_cur))
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