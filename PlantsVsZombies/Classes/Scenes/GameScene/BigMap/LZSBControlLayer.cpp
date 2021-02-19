/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.14
 *Email: 2117610943@qq.com
 */

#include "LZSBControlLayer.h"
#include "LZSBBigMapGameScene.h"
#include "../LZSGData.h"
#include "../LZSGDefine.h"
#include "../LZSGZombiesAppearControl.h"
#include "../LZSGAnimationLayer.h"
#include "../LZSGButtonLayer.h"
#include "../LZSGInformationLayer.h"
#include "../LZSGGameEndLayer.h"
#include "Scenes/EasterEggsScene/LZSEGameEasterEggs.h"
#include "Based/LZBLevelData.h"
#include "Based/LZBPlayMusic.h"
#include "Based/LZBUserData.h"
#include "Plants/LZPPlants.h"
#include "Zombies/LZZZombies.h"

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
	MAP_INIT(gameMapInformation->plantsMap);
	gameMapInformation->mapLeft = GRASS_BIGMAP_POSITION_LEFT;
	gameMapInformation->mapRight = GRASS_BIGMAP_POSITION_RIGHT;
	gameMapInformation->mapTop = GRASS_BIGMAP_POSITION_TOP;
	gameMapInformation->mapBottom = GRASS_BIGMAP_POSITION_BOTTOM;

	_zombiesAppearControl = new ZombiesAppearControl();
	_levelData = _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getMunchZombiesFrequency();
}

void BMControlLayer::createSchedule()
{
	schedule([&](float) {
		controlCardEnabled();
		createZombies();
		controlRefurbishMusicAndText();
		judgeLevelIsFinished();
		}, 0.1f, "mainUpdate");

	schedule([&](float) {
		zombiesComeTiming();
		}, 1.0f, "zombiesComing");
}

void BMControlLayer::calculatePlantPosition()
{
	/* 如果不在范围内，移除到画面外 */
	if (GRASS_BIGMAP_OUTSIDE(addScrollViewOffset(_cur)))
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
			
			if (GRASS_BIGMAP_INSIDE(addScrollViewOffset(_cur), i, j) &&
				(_plantsPosition.x != j || _plantsPosition.y != i))
			{
				_plantsPosition.x = j;
				_plantsPosition.y = i;
			}
		}
	}
}

void BMControlLayer::createMouseListener()
{
	/* 创建鼠标监听 */
	_listener = EventListenerMouse::create();
	_listener->setEnabled(false);

	/* 鼠标移动 */
	_listener->onMouseMove = [&](Event* event)
	{
		/* 获取鼠标位置 */
		_cur = static_cast<EventMouse*>(event)->getLocationInView();
		calculatePlantPosition();
		mouseMoveControl();
		showSelectedButtonHoverEffect();
		//changeScrollViewOffset();
	};

	/* 鼠标按下 */
	_listener->onMouseDown = [&](Event* event)
	{
		_cur = static_cast<EventMouse*>(event)->getLocationInView();
		mouseDownControl(static_cast<EventMouse*>(event));
	};

	_listener->onMouseScroll = [&](Event* event)
	{
		float scrollY = static_cast<EventMouse*>(event)->getScrollY() * 300;
		auto offset = BigMapGameScene::scrollView->getContentOffset();
		offset = Vec2(offset.x, offset.y + scrollY);
		if (offset.y > 0)offset.y = 0;
		if (offset.y < -1080)offset.y = -1080;
		BigMapGameScene::scrollView->setContentOffsetInDuration(offset, 0.1f);
	};

	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);
	runAction(Sequence::create(DelayTime::create(3.1f), 
		CallFunc::create([=]() 
			{
				_listener->setEnabled(true);
			}), nullptr));
}

Vec2 BMControlLayer::addScrollViewOffset(Vec2& vec2)
{
	auto offset = BigMapGameScene::scrollView->getContentOffset();
	return Vec2(vec2.x + fabs(offset.x), vec2.y + fabs(offset.y));
}

void BMControlLayer::changeScrollViewOffset()
{
	_offset = BigMapGameScene::scrollView->getContentOffset();

	if (_cur.x > 230 && _cur.x < 530)
	{
		this->runAction(RepeatForever::create(Sequence::create(
			CallFunc::create([&]()
				{
					_offset = Vec2(_offset.x + 1, _offset.y);
					if (_offset.y < 0)_offset.y = 0;
					BigMapGameScene::scrollView->setContentOffset(_offset);
				}), DelayTime::create(0.02f), nullptr)));
	}
	else if (_cur.x < 1920 && _cur.x > 1620)
	{
		this->runAction(RepeatForever::create(Sequence::create(
			CallFunc::create([&]()
				{
					_offset = Vec2(_offset.x - 1, _offset.y);
					if (_offset.y < -1080)_offset.y = -1080;
					BigMapGameScene::scrollView->setContentOffset(_offset);
				}), DelayTime::create(0.02f), nullptr)));
	}
	else if (_cur.y > 0 && _cur.y < 300)
	{
		this->runAction(RepeatForever::create(Sequence::create(
			CallFunc::create([&]()
				{
					_offset = Vec2(_offset.x, _offset.y + 1);
					if (_offset.y > 0)_offset.y = 0;
					BigMapGameScene::scrollView->setContentOffset(_offset);
				}), DelayTime::create(0.02f), nullptr)));
	}
	else if (_cur.y < 1080 && _cur.y > 780 && _cur.x > 220)
	{
		this->runAction(RepeatForever::create(Sequence::create(
			CallFunc::create([&]()
				{
					_offset = Vec2(_offset.x, _offset.y - 1);
					if (_offset.y < -1080)_offset.y = -1080;
					BigMapGameScene::scrollView->setContentOffset(_offset);
				}), DelayTime::create(0.02f), nullptr)));
	}
	else
	{
		this->stopAllActions();
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
	_plantCurImage->setPosition(addScrollViewOffset(_cur));
	_plantCurImage->setGlobalZOrder(10);
}

void BMControlLayer::mouseDownControl(EventMouse* eventmouse)
{
	switch (eventmouse->getMouseButton())
	{
	case EventMouse::MouseButton::BUTTON_RIGHT:
		break;
	case EventMouse::MouseButton::BUTTON_LEFT:
		mouseLeftButtonDownControl();
		break;
	case EventMouse::MouseButton::BUTTON_MIDDLE:
		mouseRightButtonDownControl();
		break;
	default:
		break;
	}

	if (_selectPlantsTag != PlantsType::None)
	{
		buttonLayerInformation->plantsCards[static_cast<unsigned int>
			(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setColor(Color3B::WHITE);
		buttonLayerInformation->plantsCards[static_cast<unsigned int>
			(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setVisible(false);
	}
}

void BMControlLayer::mouseMoveControl()
{
	/* 如果鼠标选择了植物 */
	if (buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
		int posX = static_cast<int>(_plantsPosition.x);
		int posY = static_cast<int>(_plantsPosition.y);
		if (posX >= 0 && posY >= 0 && posX < 18 && posY < 10)
		{
			if (gameMapInformation->plantsMap[posY][posX] != NO_PLANTS)
			{
				_plantPreviewImage->setPosition(SET_OUT_MAP);
			}
			else
			{
				auto size = _plantPreviewImage->getContentSize() / 2.f;
				_plantPreviewImage->setPosition(Vec2(GRASS_BIGMAP_POSITION_LEFT + 121 *
					_plantsPosition.x + size.width, GRASS_BIGMAP_POSITION_BOTTOM + 136 * (_plantsPosition.y + 1) - size.height));
			}
		}
		else
		{
			_plantPreviewImage->setPosition(SET_OUT_MAP);
		}
		_plantCurImage->setPosition(addScrollViewOffset(_cur) + Vec2(0, 30));
	}

	/* 鼠标上有铲子 */
	if (buttonLayerInformation->mouseSelectImage->isSelectShovel)
	{
		/* 循环把植物恢复到原来的颜色 */
		recoveryPlantsColor();

		if (judgeMousePositionIsInMap() && judgeMousePositionHavePlant())  /* 如果在地图范围内 && 种有植物 */
		{
			auto plant = animationLayerInformation->getChildByTag(SET_TAG(_plantsPosition));
			if (plant)
			{
				plant->setColor(Color3B(100, 100, 100));
			}
		}
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

		if (_global->userInformation->getCurrentPlayLevels() >= 52)
		{
			_director->getInstance()->pushScene(TransitionFade::create(0.5f, GameEasterEggs::create()));
		}
		_gameEndShieldLayer->successfullEntry();
	}
}

void BMControlLayer::judgeZombiesWin(Zombies* zombie)
{
	if (!isLose && zombie->getZombieAnimation()->getPositionX() < GRASS_BIGMAP_POSITION_LEFT - 100)
	{
		isLose = true;
		auto gameEndShieldLayer = GSGameEndLayer::create();
		Director::getInstance()->getRunningScene()->addChild(gameEndShieldLayer, 10, "gameEndShieldLayer");
		gameEndShieldLayer->breakThrough(GameTypes::UserLose);
	}
}