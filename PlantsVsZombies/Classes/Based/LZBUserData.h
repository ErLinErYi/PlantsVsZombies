/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.20
 *Emal: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "json/writer.h"
#include "json/document.h"
#include "json/stringbuffer.h"

#include <string>
#include "base/CCDirector.h"

//#define DEBUG 
//#define DLLTEST

using namespace rapidjson;
using namespace cocos2d;
using namespace std;

class Global;
enum class openUserDataReturnType
{
    FileExistError = 1,
    FileExistCorrect,
    FileNotExist
};

struct GSScene
{
    GSScene() :
        scene(nullptr),
        worldId(0),
        levelId(0)
    {}
    Scene* scene;
    int worldId;
    int levelId;
};

class UserData :public Director
{
public:
    static UserData* getInstance();
    void flushUserData();
    void flushLevelData();
    void flushSurvivalData();

    void caveUserData(char* key, double value);
    void caveUserData(char* key, bool value);
    void caveUserData(char* key, char* value);
    void caveUserData(char* key, int value);

    void caveLevelData(char* key);
    void openLevelData(char* key);
    bool isHaveLevelData(char* key);

    void caveSurvivalData(char* key);
    void openSurvivalData(char* key);
    bool isHaveSurvivalData(char* key);

    void openLevelPlantsData(char* key);
    void openLevelZombiesData(char* key);
    void openLevelSelectCardData(char* key);
    void openLevelSunData(char* key);
    void openLevelCoinData(char* key);
    void openLevelCarData(char* key);
    void openLevelBulletData(char* key);
    void openLevelOtherData(char* key);
    void openSurvivalOtherData(char* key);

    void removeLevelData(char* key);

    int openIntUserData(char* key);
    double openDoubleUserData(char* key);
    bool openBoolUserData(char* key);
    const char* openStringUserData(char* key);

    void createNewUserDataDocument();
    void createNewLevelDataDocument();
    void setAnewReadData(const bool newRead);

private:
    UserData();
    ~UserData();
    string getUserDataFileName();
    string getLevelDataFileName();
    string getSurvivalDataFileName();
    openUserDataReturnType openUserData();
    openUserDataReturnType openLevelData();
    openUserDataReturnType openSurvivalData();

    bool isHaveMember(char* key);
    bool readLevelData();

    void caveLevelPlantsData(char* key);
    void caveLevelZombiesData(char* key);
    void caveLevelSelectPlantsData(char* key);
    void caveLevelSunData(char* key);
    void caveLevelCoinData(char* key);
    void caveLevelCarData(char* key);
    void caveLevelBulletData(char* key);
    void caveLevelOtherData(char* key);
    void caveSurvivalOtherData(char* key);

    void replaceScene();
#ifndef DLLTEST
    string encryption(string& str);
    string decryption(string& str);
#endif // !DLLTEST

private:
    Document* _userDataDocument;
    Document* _levelDataDocument;
    FileUtils* _fileUtils;
    Global* _global;
    string _userData;
    string _levelData;
    bool _isAnewReadData;
    vector<GSScene*>_gsScene;
    static UserData* _instance;
};