/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.24
 *Email: 2117610943@qq.com
 */

#pragma once

#include <string>

class PlayMusic
{
public:
    static void playMusic(const std::string& musicName,const bool isControlVolume = true);
    static int playMusic(const std::string& musicName,const int);
    static int changeBgMusic(const std::string& _musicName, bool _loop);
    static void setMusicVolume(const int audioId);
    static void stopMusic();
    static void resumeMusic();
};