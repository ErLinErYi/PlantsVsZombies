/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Email: 2117610943@qq.com
 */

#pragma once
#include "ui/CocosGUI.h"
#include "LZUserInformation.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#define USEFONT(__FONT__) Global::getInstance()->userInformation->getGameText().find(__FONT__)->second->text
#define GAME_FONT_NAME_1 USEFONT("TTF字体名称1")
#define GAME_FONT_NAME_2 USEFONT("TTF字体名称2")
#define GAME_FONT_NAME_3 USEFONT("TTF字体名称3")

#define MYRELEASE 1

#define COUNTPLAYERS                                                                                                                                    \
HttpRequest * request = new HttpRequest();                                                                                                              \
request->setUrl("http://api.k780.com/?app=code.punycode_encode&str=%E7%99%BE%E5%BA%A6&appkey=60122&sign=d4666cf57fc0d3a916e2c9d0a1b25c68&format=json"); \
request->setRequestType(HttpRequest::Type::GET);                                                                                                        \
HttpClient::getInstance()->send(request);                                                                                                               \

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
