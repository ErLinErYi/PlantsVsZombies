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

MomentTime::MomentTime() :m_second(0), m_day(0), m_hour(0), m_year(0), m_minute(0), m_moth(0), _onlyNetTime(false), _netTimeResquest(false)
{
	_downloader.reset(new network::Downloader());
}

MomentTime::~MomentTime()
{
}

void MomentTime::requestNetTime()
{
	const std::string sURLList = "http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp";
	_downloader->createDownloadDataTask(sURLList);
	_downloader->onDataTaskSuccess = [this](const cocos2d::network::DownloadTask& task,
		std::vector<unsigned char>& data)
	{
		if (!_netTimeResquest)
		{
			std::string str;
			for (auto p : data)
			{
				str += p;
			}

			if (!readJson(str))
			{
				getLocalTime();
			}

			if (netTimeCallBack)
			{
				netTimeCallBack();
			}

			_netTimeResquest = true;
		}
	};

	_downloader->onTaskError = [this](const cocos2d::network::DownloadTask& task,
		int errorCode,
		int errorCodeInternal,
		const std::string& errorStr)
	{
		if (!_netTimeResquest)
		{
			getLocalTime();

			if (netTimeCallBack)
			{
				netTimeCallBack();
			}

			_netTimeResquest = true;
		}
	};

	auto node = Director::getInstance()->getRunningScene();
	if (node)
	{
		node->runAction(Sequence::create(DelayTime::create(5.f),
			CallFunc::create([this]()
				{
					if (!_netTimeResquest)
					{
						getLocalTime();

						if (netTimeCallBack)
						{
							netTimeCallBack();
						}

						_netTimeResquest = true;
					}
				}), nullptr));
	}
}

void MomentTime::requestNetTime(const std::function<void()>& pSelector, bool onlyNetTime)
{
	requestNetTime();

	netTimeCallBack = pSelector;
	_onlyNetTime = onlyNetTime;
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
		if (doc.HasMember("data"))
		{
			auto& temp = doc["data"];
			if (temp.HasMember("t"))
			{
				std::string str = temp["t"].GetString();
				auto s = str.substr(0, str.size() - 3);
				m_time = atoi(s.c_str());
				initNetTime(m_time);

				return true;
			}
		}
	}
	return false;
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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	struct timeval now;

	if (!_onlyNetTime)
	{
		gettimeofday(&now, NULL);
	}

	initTime(localtime(&now.tv_sec));
#endif

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	time_t tt;

	if (!_onlyNetTime)
	{
		time(&tt);
	}

	initTime(localtime(&tt));
#endif
}

void MomentTime::initTime(struct tm* netTime)
{
	if (netTime)
	{
		m_year = netTime->tm_year + 1900;
		m_moth = netTime->tm_mon + 1;
		m_day = netTime->tm_mday;
		m_hour = netTime->tm_hour;
		m_minute = netTime->tm_min;
		m_second = netTime->tm_sec;
		m_week = netTime->tm_wday == 0 ? 7 : netTime->tm_wday;
	}
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