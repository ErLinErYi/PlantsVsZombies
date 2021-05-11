/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.14
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "Based/LZDialog.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class Global;
class UserWinRequirement;
class OpenLevelData;
enum class GameTypes;

class GSGameEndLayer :public Dialog
{
public:
    CREATE_FUNC(GSGameEndLayer);
    static void judgeBreakThroughAboutJumpLevel();
    virtual void successfullEntry();
    virtual void breakThrough(GameTypes gameType);

CC_CONSTRUCTOR_ACCESS:
    GSGameEndLayer();
    ~GSGameEndLayer();
    virtual bool init() override;

protected:
    virtual void caveLevelNumber();
    virtual void showFailDialog(GameTypes gameType);
    virtual void showFailText();
    virtual void carsToCoins();
    virtual void rewardCoin(Button* button);
    virtual void coinAction(const Vec2& position, const int id, const bool big = false);
    virtual void rewardThing();
    virtual void quitScene();

protected:
    Global* _global;
    UserWinRequirement* _userWinRequirement;
    OpenLevelData* _openLevelData;
};