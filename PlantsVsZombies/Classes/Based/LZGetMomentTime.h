/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.11
 *Emal: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "network/HttpClient.h"

using namespace cocos2d;
using namespace network;

class MomentTime : public Node
{
public:
	CREATE_FUNC(MomentTime);

	void requestNetTime();
	void requestNetTime(const std::function<void()>& pSelector);
	int getNetDay();
	int getNetMon();
	int getNetYear();
	int getNetHour();
	int getNetSec();
	int getNetMin();
	int getNetWeek();
	time_t& GetNetTime();
	std::function<void()> netTimeCallBack;

CC_CONSTRUCTOR_ACCESS:
	MomentTime();
	~MomentTime();

private:
	void onHttpComplete(HttpClient* sender, HttpResponse* response);
	bool readJson(std::string jsonStr);
	bool splitString(const std::string content, std::string pattern);
	void initNetTime(time_t tmTime);
	void initTime(struct tm* netTime);
	void getLocalTime();

protected:
	int m_day;
	int m_moth;
	int m_year;
	int m_hour;
	int m_second;
	int m_minute;
	int m_week;
	time_t m_time;
};