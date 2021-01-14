#pragma once
#include "cocos2d.h"
#include "json/writer.h"
#include "json/document.h"
#include "json/stringbuffer.h"

using namespace std;
using namespace cocos2d;
using namespace rapidjson;

class LevelData;

class OpenLevelData
{
public:
	/**
	 *单例
	 */
	static OpenLevelData* getInstance();

	/**
	 *打开关卡数据
	 */
	bool openLevelsData(const string& worlddata);

	/*
	 *解密关卡数据
	 */
	void decrypt(char* cSrc, char* cDest);
	void decrypt(string& cSrc, char* cDest);

	/**
	 *获取所有关卡数据
	 */
	Document* getDocument();

	/**
	 *创建某一个关卡数据
	 */
	void createLevelData(const int level, const char* levelName);

	/**
	 *读取某一个关卡
	 */
	LevelData* readLevelData(const int level);

	/**
	 *设置关卡数
	 */
	void setLevelNumber(const int levelNumber);

	/**
	 *获取关卡数
	 */
	int getLevelNumber() const;

	/**
	 初始化
	 */
	void documentInit();

private:
	OpenLevelData():_document(new Document), _levelNumber(-1){}
	~OpenLevelData() {}

private:
	static OpenLevelData* _instance;
	Document* _document;
	map<int, LevelData*>_levelData;
	int _levelNumber;
};

struct MyPoint
{
	MyPoint():x(0),y(0){}
	int x, y;
};

class LevelData
{
public:
	bool readLevelData(const char* LevelName);
	bool getZombiesVisible() const { return _zombiesIsVisible; }
	bool getZombiesIsSmall() const { return _zombiesIsSmall; }
	bool getZombiesIsBig() const { return _zombiesIsBig; }
	bool getIsNoPlants() const { return _isNoPlants; }
	int getZombiesFrequency() const { return _zombiesFrequency; }
	int getCoinNumbers() const { return _coinNumbers; }
	int getAtLeastSunNumbers() const { return _atLeastSunNumbers; }
	int getFlowerPosition() const { return _flowerPosition; }
	int getCarNumbers() const { return _carNumbers; }
	int getUsePlantsNumbers() const { return _usePlantsNumbers; }
	int getFirstFrequencyTime() const { return _firstFrequencyTime; }
	float getUserLostPosition() const { return _userLose; }
	vector<int>& getGameType() { return _gameType; }
	vector<int>& getZombiesType() { return _zombiesType; }
	vector<int>& getZombiesNumbers() { return _zombiesNumbers; }
	vector<int>& getMunchZombiesFrequency() { return _munchZombiesFrequency; }
	vector<MyPoint>& getNoPlantsPosition() { return _noPlantsPosition; }
	vector<vector<int> >& getZombiesTypeProbabilityFrequency() { return _zombiesTypeProbabilityFrequency; }

CC_CONSTRUCTOR_ACCESS:
	LevelData();
	~LevelData();

private:
	void setGameTypes(const char* LevelName);
	
private:
	bool _isEncryption;                                              /* 是否加密 */
	bool _zombiesIsVisible;                                          /* 僵尸是否隐身 */
    bool _zombiesIsSmall;                                            /* 是否是小僵尸 */
	bool _zombiesIsBig;                                              /* 是否是巨人僵尸 */
	bool _isNoPlants;                                                /* 是否不可种植 */
	int _zombiesFrequency;                                           /* 僵尸总波数 */
	int _coinNumbers;                                                /* 金币数 */
	int _atLeastSunNumbers;                                          /* 至少产生的阳光数 */
	int _flowerPosition;                                             /* 花坛位置 */
	int _carNumbers;                                                 /* 小车数量 */
	int _usePlantsNumbers;                                           /* 使用植物数量 */
	int _firstFrequencyTime;                                         /* 第一波僵尸出现时间 */
	float _userLose;                                                 /* 玩家失败 */
	vector<int>_gameType;                                            /* 游戏类型 */
    vector<int>_zombiesType;                                         /* 僵尸类型 */
	vector<int>_zombiesNumbers;                                      /* 僵尸数 */
	vector<int>_munchZombiesFrequency;                               /* 多僵尸波数 */
	vector<vector<int> >_zombiesTypeProbabilityFrequency;            /* 每一波每种僵尸出现的概率 */
	vector<MyPoint>_noPlantsPosition;                                /* 不可以种植的地方 */
	Document* _document;
};