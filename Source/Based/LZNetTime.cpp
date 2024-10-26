/**
 *Copyright (c) 2024 LZ.All Right Reserved
 *Author : LZ
 *Date: 2024.10.26 22:10
 *Emal: 2117610943@qq.com
 */

#include "LZNetTime.h"

#include "axmol.h"
#include "rapidjson/document.h"

NetTime::NetTime()
    : _second{0}
    , _minute{0}
    , _hour{0}
    , _day{0}
    , _moth{0}
    , _year{0}
    , _week{0}
    , _onlyNetTime{false}
    , _alreadyLoad{false}
    , _netTimeCallBack{nullptr}
    , _downloader{std::make_unique<ax::network::Downloader>()}
{}

void NetTime::loadTime()
{
    constexpr std::string_view urlList = "https://api.pinduoduo.com/api/server/_stm";
    _downloader->createDownloadDataTask(urlList);
    _downloader->onDataTaskSuccess = [this](const ax::network::DownloadTask& task, std::vector<unsigned char>& data) {
        if (!_alreadyLoad)
        {
            std::string str(data.cbegin(), data.cend());
            if (!readJsonData(str))
            {
                getLocalTime();
            }

            if (_netTimeCallBack)
            {
                _netTimeCallBack();
            }

            _alreadyLoad = true;
        }
    };

    _downloader->onTaskError = [this](const ax::network::DownloadTask& task, int errorCode, int errorCodeInternal,
                                      std::string_view errorStr) {
        if (!_alreadyLoad)
        {
            getLocalTime();

            if (_netTimeCallBack)
            {
                _netTimeCallBack();
            }

            _alreadyLoad = true;
        }
    };

    if (auto node = ax::Director::getInstance()->getRunningScene())
    {
        node->runAction(ax::Sequence::create(ax::DelayTime::create(5.f), ax::CallFunc::create([this]() {
            if (!_alreadyLoad)
            {
                getLocalTime();

                if (_netTimeCallBack)
                {
                    _netTimeCallBack();
                }

                _alreadyLoad = true;
            }
        }),
                                             nullptr));
    }
}

void NetTime::loadTime(const std::function<void()>& pf, bool onlyNetTime)
{
    _netTimeCallBack = pf;
    _onlyNetTime     = onlyNetTime;

    loadTime();
}

bool NetTime::readJsonData(std::string_view jsonStr)
{
    rapidjson::Document doc;
    doc.Parse(jsonStr.data());
    if (doc.IsObject())
    {
        if (doc.HasMember("server_time"))
        {
            getNetTime(doc["server_time"].GetInt64() / 1000);
            return true;
        }
    }
    return false;
}

void NetTime::getNetTime(time_t tmTime)
{
    init(localtime(&tmTime));
}

void NetTime::getLocalTime()
{
#if (AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID || AX_TARGET_PLATFORM == AX_PLATFORM_IOS)
    struct timeval now;

    if (!_onlyNetTime)
    {
        gettimeofday(&now, NULL);
    }

    time_t now_sec = now.tv_sec;
    initTime(localtime(&now_sec));
#endif

#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32)
    time_t tt{0};
    if (!_onlyNetTime)
    {
        time(&tt);
    }

    init(localtime(&tt));
#endif
}

void NetTime::init(struct tm* netTime)
{
    if (netTime)
    {
        _year   = netTime->tm_year + 1900;
        _moth   = netTime->tm_mon + 1;
        _day    = netTime->tm_mday;
        _hour   = netTime->tm_hour;
        _minute = netTime->tm_min;
        _second = netTime->tm_sec;
        _week   = netTime->tm_wday == 0 ? 7 : netTime->tm_wday;
    }
}
