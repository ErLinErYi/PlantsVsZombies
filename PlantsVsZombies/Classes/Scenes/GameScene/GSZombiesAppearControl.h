/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include <random>

#include "Zombies/Zombies.h"
#include "Based/GlobalVariable.h"

 /* 僵尸出现波数控制 */
class ZombiesAppearControl
{
public:
	/**
	 *对于进攻的不同波数创建不同数量僵尸
	 */
	int getZombiesNumbersForAppearFrequency(const unsigned int& ZombiesAppearFrequency);

	/**
	 *获取僵尸波数
	 */
	int getZombiesAppearFrequency() const;

	/**
	 *获取上一波僵尸是否已经阵亡
	 */
	bool getLastFrequencyZombiesWasDeath() const;

	/**
	 *获取计时是否开始
	 */
	bool getIsBegin() const;

	/**
	 *获取是否显示文字
	 */
	bool getIsShowWords() const;

	/**
	 *获取时间
	 */
	double getTime() const;

	/**
	 *使每一行的僵尸等概率出现
	 */
	int getEqualProbabilityForRow();

	/**
	 *设置僵尸出现波数
	 * 1.传递参数设置
	 */
	void setZombiesAppearFrequency(const unsigned int zombiesAppearFrequency);

	/**
	 *设置僵尸出现波数
	 * 2.自增加一
	 */
	void setZombiesAppearFrequency();

	/**
	 *设置上一波僵尸是否死亡
	 */
	void setLastFrequencyZombiesWasDeath(bool lastFrequencyZombiesWasDeath);

	/**
	 *设置时间清零
	 */
	void setTimeClear();

	/**
	 *设置时间自增
	 */
	void setTimeAdd();
	void setTimeAdd(const double t);

	/**
	 *设置是否开始
	 */
	void setIsBegin(const bool isBegin);

	/**
	 *设置是否显示文字
	 */
	void setIsShowWords(const bool isShowWords);

	/**
	 *创建不同种类的僵尸
	 */
	ZombiesType createDifferentTypeZombies(const unsigned int& ZombiesAppearFrequency);

	/**
	 *创建不同种类的僵尸
	 */
	ZombiesType createDifferentTypeZombies();

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
	unsigned int _zombiesPosition[5] =         // 僵尸位置
	{
		130,268,406,544,682
	};
};