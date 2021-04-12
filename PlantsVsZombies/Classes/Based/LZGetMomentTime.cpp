/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.4.11
 *Emal: 2117610943@qq.com
 */

#include "LZGetMomentTime.h"
#include "json/document.h"
#include <regex>

using namespace rapidjson;

MomentTime::MomentTime() :m_second(0), m_day(0), m_hour(0), m_year(0), m_minute(0), m_moth(0)
{
}

MomentTime::~MomentTime()
{
}

void MomentTime::requestNetTime()
{
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://api.k780.com:88/?app=life.time&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json"); //百度获取时间的api

	request->setRequestType(HttpRequest::Type::GET);
	request->setTag("WebTime");
	request->setResponseCallback(this, httpresponse_selector(MomentTime::onHttpComplete));
	HttpClient::getInstance()->send(request);

	request->release();
}

void MomentTime::requestNetTime(const std::function<void()>& pSelector)
{
	requestNetTime();

	netTimeCallBack = pSelector;
}

void MomentTime::onHttpComplete(HttpClient* sender, HttpResponse* response)
{
	if (!response)return;

	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		CCLOG("%s completed", response->getHttpRequest()->getTag());
	}

	if (!response->isSucceed())
	{
		CCLOG("response failed");
		CCLOG("error buffer:%s", response->getErrorBuffer());
		getLocalTime();

		if (netTimeCallBack)
		{
			netTimeCallBack();
		}
		return;
	}
	std::vector<char>* buffer = response->getResponseData();
	std::string str;
	for (unsigned i = 0; i < buffer->size(); i++)
	{
		char a = (*buffer)[i];
		str.append(1, a);
	}

	if (!readJson(str))
	{
		getLocalTime();
	}

	if (netTimeCallBack)
	{
		netTimeCallBack();
	}
}

bool MomentTime::readJson(std::string jsonStr)
{
	Document doc;
	doc.Parse<0>(jsonStr.c_str());
	if (doc.IsObject())
	{
		if (doc.HasMember("result"))
		{
			auto& temp = doc["result"];
			if (temp.HasMember("timestamp"))
			{
				m_time = atoi(temp["timestamp"].GetString());
				initNetTime(m_time);

				return true;
			}
		}
	}
	return splitString(jsonStr, "timestamp");
}

bool MomentTime::splitString(const std::string content, std::string pattern)
{
	std::string contentVector;
	pattern = "(?:timestamp\":\")\\d{10}";
	pattern = "[[:alnum:]]*" + pattern + "[[:alnum:]]*";
	std::regex r(pattern);
	std::smatch what;
	if (regex_search(content, what, r))
	{
		contentVector = what.str();
		auto string_size = contentVector.size();
		contentVector = contentVector.substr(string_size - 10, 10);
		m_time = atoi(contentVector.c_str());
		initNetTime(m_time);

		return true;
	}
	return false;
}

void MomentTime::initNetTime(time_t tmTime)
{
	initTime(localtime(&tmTime));
}

void MomentTime::getLocalTime()
{
	time_t tt;
	time(&tt);

	initTime(localtime(&tt));
}

void MomentTime::initTime(struct tm* netTime)
{
	m_year = netTime->tm_year + 1900;
	m_moth = netTime->tm_mon + 1;
	m_day = netTime->tm_mday;
	m_hour = netTime->tm_hour;
	m_minute = netTime->tm_min;
	m_second = netTime->tm_sec;
	m_week = netTime->tm_wday == 0 ? 7 : netTime->tm_wday;
}

int MomentTime::getNetDay()
{
	return m_day;
}

int MomentTime::getNetMon()
{
	return m_moth;
}

int MomentTime::getNetYear()
{
	return m_year;
}

int MomentTime::getNetHour()
{
	return m_hour;
}

int MomentTime::getNetSec()
{
	return m_second;
}

int MomentTime::getNetMin()
{
	return m_minute;
}

int MomentTime::getNetWeek()
{
	return m_week;
}

time_t& MomentTime::GetNetTime()
{
	return m_time;
}