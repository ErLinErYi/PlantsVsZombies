/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Email: 2117610943@qq.com
 */

#pragma once
#include "ui/CocosGUI.h"
#include "LZUserInformation.h"
#include "Based/LZDefine.h"

using namespace std;
using namespace cocos2d;

class Global
{
public:
	static Global* getInstance();
	static void checkUserDataFile();
	static void writeComputerUniqueIdentification();
	void checkAnimationInterval();

public:
	UserInformation* userInformation;

private:
	Global();
	~Global();
	static bool getComputerUniqueIdentification(string& identification);
	static void showWanging();

private:
	static Global* _instance;    /* 单例 */
	static string _computerUniqueIdentificationString;
	float _sumAnimationInterval;
	int _frequence;
};
