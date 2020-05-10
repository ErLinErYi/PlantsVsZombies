/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include <random>

#include "cocos2d.h"
#include "Based/GlobalVariable.h"

using namespace cocos2d;

class ZombiesAppearControl;
class GSGameEndLayer;
class OpenLevelData;
enum class ZombiesType;
enum class PlantsType;


/* 游戏地图 */
struct GameMapInformation
{
	GameMapInformation();
	int plantsMap[5][9];               /* 植物种植地图 */
	unsigned int rowNumbers;           /* 行数 */
	unsigned int columnNumbers;        /* 列数 */
};

class GSControlLayer :public Layer
{
public:
	CREATE_FUNC(GSControlLayer);
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }
	static void setPlantMapCanPlant(const unsigned int colum, const unsigned int row);
	void updateFlag();

CC_CONSTRUCTOR_ACCESS:
	GSControlLayer();
	~GSControlLayer();
	virtual bool init();

protected:
	virtual void judgeLevelIsFinished();                                                /* 判断关卡是否结束 */
	virtual void zombiesComeTiming();                                                   /* 僵尸出现计时 */
	virtual void createZombies();                                                       /* 创建僵尸 */
	virtual bool controlRefurbishZombies();                                             /* 控制僵尸刷新 */
	virtual void controlRefurbishMusicAndText();                                        /* 控制音乐与文字的刷新*/

private:
	void createSchedule();                                                              /* 创建定时器 */
	void controlCardEnabled();                                                          /* 控制卡牌是否可用 */
	void calculatePlantPosition();                                                      /* 计算植物种植位置 */
	void createMouseListener();                                                         /* 创建鼠标 */
	void mouseMoveControl();                                                            /* 鼠标移动控制 */
	void mouseDownControl(EventMouse* eventmouse = nullptr);                            /* 鼠标按下控制 */
	void createPlantsCardListener();                                                    /* 创建植物卡牌监听 */
	void selectPlantsPreviewImage();                                                    /* 选择植物预览图片 */
	void createPreviewPlants();                                                         /* 创建预览植物 */
	bool judgeMousePositionIsInMap();                                                   /* 判断鼠标点击方位是否在地图内 */
	bool judgeMousePositionIsCanPlant();                                                /* 判断鼠标点击方位是否可以种植植物 */
	bool judgeMousePositionHavePlant();                                                 /* 判断鼠标点击方位是否有植物 */
	void removePreviewPlant();                                                          /* 移除预览植物 */
	void removeShovel();                                                                /* 移除铲子 */
	void recoveryPlantsColor();                                                         /* 恢复植物颜色 */
  	void setGameEnd();                                                                  /* 设置游戏结束 */
	
public:
	Vec2 _cur;                                           // 鼠标坐标 
	Vec2 _plantsPosition;                                // 植物种植坐标
	PlantsType _selectPlantsTag;                         // 所选植物标记
	GameMapInformation* _gameMapInformation;             // 游戏地图信息
	ZombiesAppearControl* _zombiesAppearControl;         // 僵尸出现计算

protected:
	Sprite* _plantPreviewImage;                          // 植物预览图片
	Sprite* _plantCurImage;                              // 植物鼠标图片
	Global* _global;                                     // 全局单例
	Node* _animationLayer;                               // 游戏层
	GSGameEndLayer* _gameEndShieldLayer;                 // 游戏结束屏蔽层
	OpenLevelData* _openLevelData;                       // 关卡数据单例
	vector<int> _levelData;                              // 临时关卡数据
};