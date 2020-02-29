/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.14
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/GameType.h"

using namespace cocos2d;

class OpenLevelData;
class LevelData;
class Zombies;

class GSGameResultJudgement
{
public:
    GameTypes judgeUserIsWin();
    GameTypes judgeUserIsLose(list<Zombies*>::iterator& zombie);

CC_CONSTRUCTOR_ACCESS:
	GSGameResultJudgement();
	~GSGameResultJudgement();

private:
    bool getZombieIsInHouse();
    bool getZombieIsTrampleFlowers();

private:
    LevelData* _levelData;
    OpenLevelData* _openLevelData;
    list<Zombies*>::iterator _zombie;
};
