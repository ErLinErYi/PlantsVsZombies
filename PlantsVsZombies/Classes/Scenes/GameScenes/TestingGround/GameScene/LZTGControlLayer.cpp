/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.08
 *Email: 2117610943@qq.com
 */

#include "LZTGControlLayer.h"
#include "LZTGAnimationLayer.h"
#include "LZTGInformationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSZombiesAppearControl.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSAnimationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSButtonLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSInformationLayer.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSDefine.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Based/LZPlayMusic.h"

TGControlLayer::TGControlLayer()
{
}

TGControlLayer::~TGControlLayer()
{
}

bool TGControlLayer::init()
{
	if (!Layer::init())return false;

	initData();
	createSchedule();
	createPlantsCardListener();
	createMouseListener();

	return true;
}

void TGControlLayer::initData()
{
	srand(time(nullptr));
	gameMapInformation = new GameMapInformation();
	MAP_INIT(gameMapInformation->plantsMap)

	_zombiesAppearControl = new ZombiesAppearControl();
}

void TGControlLayer::createSchedule()
{
	schedule([&](float) {
		controlCardEnabled();
		createZombies();
		controlRefurbishMusicAndText();
		}, 0.1f, "mainUpdate");

	schedule([&](float) {
		zombiesComeTiming();
		}, 1.0f, "zombiesComing");
}

void TGControlLayer::createZombies()
{
	/* 刷新僵尸 */
	if (_zombiesAppearControl->getLastFrequencyZombiesWasDeath())
	{
		_zombiesAppearControl->setLastFrequencyZombiesWasDeath(false);
		_zombiesAppearControl->setTimeClear(); /* 距离上一波刷新时间清零 */
		
		const auto frequence = _zombiesAppearControl->getZombiesAppearFrequency();
		const auto number = frequence + rand() % 5;
		unsigned int zombiesNumbers = number <= 25 ? number : 20 + rand() % 5;
		zombiesNumbers += frequence / 25;
		for (unsigned int i = 0; i < zombiesNumbers; ++i)
		{
			dynamic_cast<TGAnimationLayer*>(animationLayerInformation)->createZombies(static_cast<unsigned>(frequence / 3));
		}
		/* 进攻波数自增一 */
		_zombiesAppearControl->setZombiesAppearFrequency();
		dynamic_cast<TGInformationLayer*>(informationLayerInformation)->updateText();
	}
	
	/* 控制僵尸的刷新 */
	if (controlRefurbishZombies())
	{
		_zombiesAppearControl->setLastFrequencyZombiesWasDeath(true);
		_zombiesAppearControl->setIsBegin(false);
	}
}

bool TGControlLayer::controlRefurbishZombies()
{
	if ((Zombies::getZombiesNumbers() <= 4 &&
		_zombiesAppearControl->getZombiesAppearFrequency() > 3)                    /* 如果活着的僵尸数小于规定，刷新下一波 */

		|| (Zombies::getZombiesNumbers() <= 0 &&                                   /* 如果没有存活僵尸则立即刷新僵尸 */
			_zombiesAppearControl->getZombiesAppearFrequency() >= 1)

		|| (_zombiesAppearControl->getTime() >= 10 &&
			_zombiesAppearControl->getZombiesAppearFrequency() == 0)               /* 第一波刷新控制 */

		|| (_zombiesAppearControl->getTime() >= 30 &&
			_zombiesAppearControl->getZombiesAppearFrequency() > 0)                /* 如果大于40秒刷新下一波 */
		)
	{
		return true;
	}
	return false;
}

void TGControlLayer::controlRefurbishMusicAndText()
{
	/* 控制大波僵尸来袭的文字与音乐 */
	if (_zombiesAppearControl->getTime() >= 10 && _zombiesAppearControl->getZombiesAppearFrequency() == 0)
	{
		PlayMusic::playMusic("awooga");
	}
}

void TGControlLayer::selectPlantsPreviewImage()
{
	switch (buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
	case true:
		PlayMusic::playMusic("tap2");
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].progressTimer->setPercentage(0);
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->setColor(Color3B::WHITE);

		removePreviewPlant();
		break;
	case false:
		PlayMusic::playMusic("seedlift");

		/* 卡片变黑并出现选中效果 */
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->setColor(Color3B::GRAY);
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].progressTimer->setPercentage(100);
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setVisible(true);
		buttonLayerInformation->plantsCards[static_cast<unsigned int>(_selectPlantsTag)].plantsCards->getChildByName("seedPacketFlash")->setColor(Color3B::ORANGE);

		/* 鼠标选择标记 */
		buttonLayerInformation->mouseSelectImage->isSelectPlants = true;
		buttonLayerInformation->mouseSelectImage->selectPlantsId = _selectPlantsTag;

		createPreviewPlants();
		break;
	}
}

void TGControlLayer::mouseLeftButtonDownControl()
{
	if (buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
		if (judgeMousePositionIsInMap() && judgeMousePositionIsCanPlant() && _cur.x > CARD_BAR_RIGHT) /* 如果在地图范围内 && 可以种植植物 */
		{
			/* 种植植物 */
			animationLayerInformation->plantPlants();

			/* 地图记录种植的植物 */
			gameMapInformation->plantsMap[static_cast<unsigned int>(_plantsPosition.y)][static_cast<unsigned int>(_plantsPosition.x)] =
				static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId);

			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].progressTimer->setPercentage(0);
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCards->setColor(Color3B::WHITE);

			removePreviewPlant();
		}
		else
		{
			if (_cur.x > CARD_BAR_RIGHT)
			{
				PlayMusic::playMusic("buzzer");
				/* 卡牌颜色恢复 */
				buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].progressTimer->setPercentage(0);
				buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCards->setColor(Color3B::WHITE);

				/* 提示信息 */
				informationLayerInformation->createPromptText();

				removePreviewPlant();
			}
		}
	}
	if (buttonLayerInformation->mouseSelectImage->isSelectShovel) /* 鼠标上有铲子 */
	{
		if (judgeMousePositionIsInMap() && judgeMousePositionHavePlant())    /* 如果在地图范围内 && 种有植物 */
		{
			PlayMusic::playMusic("plant2");
			animationLayerInformation->deletePlants();/* 铲除植物 */
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

void TGControlLayer::mouseRightButtonDownControl()
{
	if (buttonLayerInformation->mouseSelectImage->isSelectPlants)/* 鼠标上有植物 */
	{
		if (_cur.x > CARD_BAR_RIGHT)
		{
			PlayMusic::playMusic("tap2");
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].progressTimer->setPercentage(0);
			buttonLayerInformation->plantsCards[static_cast<unsigned int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCards->setColor(Color3B::WHITE);

			removePreviewPlant();
		}
	}

	if (buttonLayerInformation->mouseSelectImage->isSelectShovel) /* 鼠标上有铲子 */
	{
		removeShovel();
		recoveryPlantsColor();
	}
}
