/**
 *Copyright (c) 2024 LZ.All Right Reserved
 *Author : LZ
 *Date: 2024.10.26 22:10
 *Emal: 2117610943@qq.com
 */

#pragma once
#include "network/HttpClient.h"
#include "network/Downloader.h"

class NetTime 
{
public:
    void loadTime();
    void loadTime(const std::function<void()>& pf, bool onlyNetTime = false);
    int getDay() const noexcept { return _day; }
    int getMonth() const noexcept { return _moth; }
    int getYear() const noexcept { return _year; }
    int getHour() const noexcept { return _hour; }
    int getSecond() const noexcept { return _second; }
    int getMinute() const noexcept { return _minute; }
    int getWeek() const noexcept { return _week; }

    NetTime();
    ~NetTime() = default;

private:
    bool readJsonData(std::string_view jsonStr);
    void getNetTime(time_t tmTime);
    void getLocalTime();
    void init(struct tm* netTime);

private:
    int _second;
    int _minute;
    int _hour;
    int _day;
    int _moth;
    int _year;
    int _week;
    bool _onlyNetTime;
    bool _alreadyLoad;
    std::function<void()> _netTimeCallBack;
    std::unique_ptr<ax::network::Downloader> _downloader;
};
