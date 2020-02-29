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
	int plantsMap[5][9];      /* 植物种植地图 */
	int rowNumbers;           /* 行数 */
	int columnNumbers;        /* 列数 */
};

class GSControlLayer :public Layer
{
public:
	CREATE_FUNC(GSControlLayer);
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	GSControlLayer();
	~GSControlLayer();
	virtual bool init();

private:
	void createSchedule();                                                              /* 创建定时器 */
	void createZombiesHowlSoundEffect();                                                /* 创建僵尸嚎叫音效 */
	void controlCardEnabled();                                                          /* 控制卡牌是否可用 */
	void calculatePlantPosition();                                                      /* 计算植物种植位置 */
	void createMouseListener();                                                         /* 创建鼠标 */
	void mouseMoveControl();                                                            /* 鼠标移动控制 */
	void mouseDownControl(EventMouse* eventmouse = nullptr);                            /* 鼠标按下控制 */
	void createPlantsCardListener();                                                    /* 创建植物卡牌监听 */
	void selectPlantsPreviewImage();                                                    /* 选择植物预览图片 */
	void createPreviewPlants();                                                         /* 创建预览植物 */
	void zombiesComeTiming();                                                           /* 僵尸出现计时 */
	void createZombies();                                                               /* 创建僵尸 */
	bool controlRefurbishZombies();                                                     /* 控制僵尸刷新 */
	void controlRefurbishMusicAndText();                                                /* 控制音乐与文字的刷新*/
	bool judgeMousePositionIsInMap();                                                   /* 判断鼠标点击方位是否在地图内 */
	bool judgeMousePositionIsCanPlant();                                                /* 判断鼠标点击方位是否可以种植植物 */
	bool judgeMousePositionHavePlant();                                                 /* 判断鼠标点击方位是否有植物 */
	void removePreviewPlant();                                                          /* 移除预览植物 */
	void removeShovel();                                                                /* 移除铲子 */
	void recoveryPlantsColor();                                                         /* 恢复植物颜色 */
	void judgeLevelIsFinished();                                                        /* 判断关卡是否结束 */
  	void setGameEnd();                                                                  /* 设置游戏结束 */
	
public:
	Vec2 _cur;                                           // 鼠标坐标 
	Vec2 _plantsPosition;                                // 植物种植坐标
	PlantsType _selectPlantsTag;                         // 所选植物标记
	GameMapInformation* _gameMapInformation;             // 游戏地图信息
	ZombiesAppearControl* _zombiesAppearControl;         // 僵尸出现计算

private:
	Sprite* _plantPreviewImage;                          // 植物预览图片
	Sprite* _plantCurImage;                              // 植物鼠标图片
	Global* _global;                                     // 全局单例
	Node* _animationLayer;                               // 游戏层
	GSGameEndLayer* _gameEndShieldLayer;                 // 游戏结束屏蔽层
	OpenLevelData* _openLevelData;                       // 关卡数据单例
	default_random_engine _random;                       // 随机数引擎
	vector<int> _levelData;                              // 临时关卡数据
};


/* 僵尸出现波数控制 */
class ZombiesAppearControl
{
public:
	/**
	 *对于进攻的不同波数创建不同数量僵尸
	 */
	int getZombiesNumbersForAppearFrequency(const int& ZombiesAppearFrequency);

	/**
	 *获取僵尸波数
	 */
	int getZombiesAppearFrequency() const { return _zombiesAppearFrequency; }

	/**
	 *获取上一波僵尸是否已经阵亡
	 */
	bool getLastFrequencyZombiesWasDeath() const { return _lastFrequencyZombiesWasDeath; }

	/**
	 *获取计时是否开始
	 */
	bool getIsBegin() const { return _isBegin; }

	/**
	 *获取是否显示文字
	 */
	bool getIsShowWords() const { return _isShowWords; }

	/**
	 *获取时间
	 */
	double getTime() const { return _time; }

	/**
	 *使每一行的僵尸等概率出现
	 */
	int getEqualProbabilityForRow();

	/**
	 *设置僵尸出现波数
	 * 1.传递参数设置
	 * 2.自增加一
	 */
	void setZombiesAppearFrequency(const int zombiesAppearFrequency) { _zombiesAppearFrequency = zombiesAppearFrequency; }
	void setZombiesAppearFrequency() { ++_zombiesAppearFrequency; }

	/**
	 *设置上一波僵尸是否死亡
	 */
	void setLastFrequencyZombiesWasDeath(bool lastFrequencyZombiesWasDeath) { _lastFrequencyZombiesWasDeath = lastFrequencyZombiesWasDeath; }

	/**
	 *设置时间清零
	 */
	void setTimeClear() { _time = 0; }

	/**
	 *设置时间自增
	 */
	void setTimeAdd() { ++_time; }

	/**
	 *设置是否开始
	 */
	void setIsBegin(const bool isBegin) { _isBegin = isBegin; }

	/**
	 *设置是否显示文字
	 */
	void setIsShowWords(const bool isShowWords) { _isShowWords = isShowWords; }

	/**
	 *创建不同种类的僵尸
	 */
	ZombiesType createDifferentTypeZombies(const int& ZombiesAppearFrequency);

CC_CONSTRUCTOR_ACCESS:
	ZombiesAppearControl();
	~ZombiesAppearControl();

private:
	int _zombiesAppearFrequency;               // 僵尸出现波次
	bool _lastFrequencyZombiesWasDeath;        // 上一波僵尸已阵亡 
	bool _isBegin;                             // 计时是否开始
	bool _isShowWords;                         // 是否显示文字
	double _time;                              // 计时
	OpenLevelData* _openLevelData;
	default_random_engine _random;             // 随机数引擎
	set<int> _judgeZombieRow;                  // 判断僵尸所在行
	int _zombiesPosition[5] =                  // 僵尸位置
	{
		130,268,406,544,682
	};
};