﻿/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.14
 *Email: 2117610943@qq.com
 */

#include "LZAGSGameResultJudgement.h"
#include "LZAGSInformationLayer.h"

#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Zombies/LZZombies.h"

GSGameResultJudgement::GSGameResultJudgement():
    _openLevelData(OpenLevelData::getInstance())
{
	_levelData = _openLevelData->readLevelData(_openLevelData->getLevelNumber());
}

GSGameResultJudgement::~GSGameResultJudgement()
{
}

bool GSGameResultJudgement::getZombieIsInHouse()
{
	return (*_zombie)->getZombieAnimation()->getPositionX() < _levelData->getUserLostPosition() ? true : false;
}

bool GSGameResultJudgement::getZombieIsTrampleFlowers()
{
	if ((*_zombie)->getZombieType() != ZombiesType::BalloonZombies &&
		(*_zombie)->getZombieAnimation()->getPositionX() < _levelData->getFlowerPosition())
	{
		return true;
	}
	return false;
}

GameTypes GSGameResultJudgement::judgeUserIsWin()
{
	for (unsigned int i = 0; i < _levelData->getGameType().size(); i++)
	{
		switch (static_cast<GameTypes>(_levelData->getGameType().at(i)))
		{
		case GameTypes::AtLeastSunNumbers:
			if (_levelData->getAtLeastSunNumbers() > informationLayerInformation->gameType->getSunNumberRequriement()->allSunNumbers)
			{
				return GameTypes::AtLeastSunNumbers;
			}
			break;
		case GameTypes::CarNumbers:
			if (CarsGroup.size() < (unsigned int)_levelData->getCarNumbers())
			{
				return GameTypes::CarNumbers;
			}
			break;
		}
	}
	return GameTypes::None;
}

GameTypes GSGameResultJudgement::judgeUserIsLose(list<Zombies*>::iterator& zombie)
{
	_zombie = zombie;

	if ((*_zombie)->getZombieIsSurvive() && (getZombieIsInHouse() || getZombieIsTrampleFlowers()))/* 僵尸存活 && （僵尸进入房间 || 僵尸踩坏花坛）*/
	{
		if (getZombieIsInHouse())
		{
			return GameTypes::UserLose;
		}
		if (getZombieIsTrampleFlowers())
		{
			return GameTypes::FlowerPosition;
		}
	}
	return GameTypes::None;
}