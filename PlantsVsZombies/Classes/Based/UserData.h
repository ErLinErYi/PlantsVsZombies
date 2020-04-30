/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.20
 *Emal: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "json/writer.h"
#include "json/prettywriter.h"
#include "json/document.h"
#include "json/stringbuffer.h"

#include <string>

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

class UserData
{
public:
    static UserData* getInstance();
    void flush();
   
    void caveUserData(char* key, double value);
    void caveUserData(char* key, bool value);
    void caveUserData(char* key, char* value);
    void caveUserData(char* key, int value);

    int openIntUserData(char* key);
    double openDoubleUserData(char* key);
    bool openBoolUserData(char* key);
    const char* openStringUserData(char* key);

    void createNewDocument();
    
private:
    UserData();
    ~UserData();
    string getUserDataFileName();
    openUserDataReturnType openUserData();
    void encrypt(string& cSrc, char* cDest);
    void decrypt(string& cSrc, char* cDest);

    bool isHaveMember(char* key);

private:
    Document *_document;
    FileUtils* _fileUtils;
    Global* _global;
    string _userData;
    static UserData* _instance;
};