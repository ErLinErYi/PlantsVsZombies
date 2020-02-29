#include "Based/GlobalVariable.h"

Global* Global::_instance = nullptr;

Global* Global::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)Global;
	}
	return _instance;
}

Global::Global():
	userInformation(new UserInformation())
{
}

Global::~Global()
{
	delete userInformation;
}

int Global::changeBgMusic(const std::string& _musicName,bool _loop)
{
	/* ÔÝÍ£ÏÈÇ°µÄ±³¾°ÒôÀÖ */
	for (auto sp : userInformation->getBackgroundMusic())
	{
		AudioEngine::stop(sp);
	}
	userInformation->getBackgroundMusic().clear();

	/* ²¥·ÅÐÂµÄ±³¾°ÒôÀÖ */
	int AudioID = AudioEngine::play2d(userInformation->getMusicPath().find(_musicName)->second, _loop);
	AudioEngine::setVolume(AudioID, userInformation->getBackGroundMusicVolume());

	userInformation->getBackgroundMusic().push_back(AudioID);

	return AudioID;
}

