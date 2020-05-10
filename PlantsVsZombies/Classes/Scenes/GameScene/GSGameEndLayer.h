/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.14
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class Global;
class UserWinRequirement;
class OpenLevelData;
enum class GameTypes;

class GSGameEndLayer :public LayerColor
{
public:
    CREATE_FUNC(GSGameEndLayer);
    void successfullEntry();
    void breakThrough(GameTypes gameType);

CC_CONSTRUCTOR_ACCESS:
    GSGameEndLayer();
    ~GSGameEndLayer();
    bool init();

private:
    void createShieldLayer();
    void showFailDialog(GameTypes gameType);
    void showFailText();
    void caveLevelNumber();
    void carsToCoins();
    void rewardCoin(Button* button);
    void coinAction(const Vec2& position, const int id, const bool big = false);
    void rewardThing();
    void quitScene();

private:
    Global* _global;
    UserWinRequirement* _userWinRequirement;
    OpenLevelData* _openLevelData;
};