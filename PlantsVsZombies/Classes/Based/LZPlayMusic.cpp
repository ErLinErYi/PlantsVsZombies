/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.24
 *Email: 2117610943@qq.com
 */

#include "AudioEngine.h"
#include "LZPlayMusic.h"
#include "LZGlobalVariable.h"

using namespace cocos2d::experimental;

void PlayMusic::playMusic(const std::string& musicName, const bool isControlVolume)
{
	if (Global::getInstance()->userInformation->getSoundEffectVolume() > 0)
	{
		if (isControlVolume)
		{
			AudioEngine::setVolume(AudioEngine::play2d(
				Global::getInstance()->userInformation->getMusicPath().find(musicName)->second),
				Global::getInstance()->userInformation->getSoundEffectVolume());
		}
		else
		{
			AudioEngine::play2d(Global::getInstance()->userInformation->getMusicPath().find(musicName)->second);
		}
	}
}

int PlayMusic::playMusic(const std::string& musicName, const int)
{
	return AudioEngine::play2d(Global::getInstance()->userInformation->getMusicPath().find(musicName)->second);
}

int PlayMusic::changeBgMusic(const std::string& _musicName, bool _loop)
{
	if (Global::getInstance()->userInformation->getBackGroundMusicVolume() > 0)
	{
		/* 暂停先前的背景音乐 */
		for (auto sp : Global::getInstance()->userInformation->getBackgroundMusic())
		{
			AudioEngine::stop(sp);
		}
		Global::getInstance()->userInformation->getBackgroundMusic().clear();

		/* 播放新的背景音乐 */
		int AudioID = AudioEngine::play2d(Global::getInstance()->userInformation->getMusicPath().find(_musicName)->second, _loop);
		AudioEngine::setVolume(AudioID, Global::getInstance()->userInformation->getBackGroundMusicVolume());

		Global::getInstance()->userInformation->getBackgroundMusic().emplace_back(AudioID);

		return AudioID;
	}
	else
	{
		return -1;
	}
}

void PlayMusic::setMusicVolume(const int audioId)
{
	AudioEngine::setVolume(audioId, Global::getInstance()->userInformation->getSoundEffectVolume());
}

void PlayMusic::stopMusic()
{
	AudioEngine::pauseAll();
}

void PlayMusic::resumeMusic()
{
	AudioEngine::resumeAll();
}
