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
class Plants;
class Zombies;
class Bullet;
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
    virtual void flushUserData();
    virtual void flushLevelData();
    
    virtual void caveUserData(char* key, double value);
    virtual void caveUserData(char* key, bool value);
    virtual void caveUserData(char* key, char* value);
    virtual void caveUserData(char* key, int value);

    virtual void caveLevelData(char* key, bool correct = false);
    virtual void openLevelData(char* key);
    virtual bool isHaveLevelData(char* key);
    virtual bool checkLevelData(char* key);

    virtual void openLevelPlantsData(char* key);
    virtual void openLevelPlantsAnimationData(char* key, const char* pl, Plants* plant);
    virtual void openLevelZombiesData(char* key);
    virtual void openLevelZombiesAnimationData(char* key, const char* zo, Zombies* zombie);
    virtual void openLevelSelectCardData(char* key);
    virtual void openLevelSunData(char* key);
    virtual void openLevelCoinData(char* key);
    virtual void openLevelCarData(char* key);
    virtual void openLevelBulletData(char* key);
    virtual void openLevelBulletAnimationData(char* key, const char* bu, Bullet* bullet);
    virtual void openLevelOtherData(char* key);
   
    virtual void removeLevelData(char* key);

    virtual int openIntUserData(char* key);
    virtual double openDoubleUserData(char* key);
    virtual bool openBoolUserData(char* key);
    virtual const char* openStringUserData(char* key);

    virtual void createNewUserDataDocument();
    virtual void createNewLevelDataDocument();
    virtual void setAnewReadData(const bool newRead);

#ifndef DLLTEST
    static string encryption(string& str);
    static string decryption(string& str);
#endif // !DLLTEST

protected:
    UserData();
    ~UserData();
    virtual string getUserDataFileName();
    virtual string getLevelDataFileName();
    virtual openUserDataReturnType openUserData();
    virtual openUserDataReturnType openLevelData();
    
    virtual bool isHaveMember(char* key);
    virtual bool readLevelData();

    virtual bool checkLevelDataVersion();
    virtual void caveLevelDataVersion();
    virtual void caveLevelPlantsData(char* key);
    virtual void caveLevelPlantsAnimationData(Plants* plant, rapidjson::Value& object);
    virtual void caveLevelZombiesData(char* key);
    virtual void caveLevelZombiesAnimationData(Zombies* zombie, rapidjson::Value& object);
    virtual void caveLevelSelectPlantsData(char* key);
    virtual void caveLevelSunData(char* key);
    virtual void caveLevelCoinData(char* key);
    virtual void caveLevelCarData(char* key);
    virtual void caveLevelBulletData(char* key);
    virtual void caveLevelBulletAnimationData(Bullet* bullet, rapidjson::Value& object);
    virtual void caveLevelOtherData(char* key);
    
    virtual void replaceScene();

    rapidjson::Value numberToString(int number, rapidjson::Document::AllocatorType& allocator);
    rapidjson::Value numberAddString(int number, string str, rapidjson::Document::AllocatorType& allocator);

protected:
    Document* _userDataDocument;
    Document* _levelDataDocument;
    FileUtils* _fileUtils;
    Global* _global;
    string _userData;
    string _levelData;
    bool _isAnewReadData;
    vector<GSScene*>_gsScene;
    static int _levelDataVersion;
    static UserData* _instance;
};